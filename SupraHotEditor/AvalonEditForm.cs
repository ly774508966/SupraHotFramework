using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;
using System.Windows.Forms;
using System.Windows.Forms.Integration;

using Microsoft.Win32;

using ICSharpCode.AvalonEdit;
using ICSharpCode.AvalonEdit.CodeCompletion;
using ICSharpCode.AvalonEdit.Folding;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using ICSharpCode.AvalonEdit.Sample;

using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Utils;
using System.Windows.Documents;

using System.Reflection;
using System.Text.RegularExpressions;
using System.Text;

using Microsoft.Scripting;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;

using SupraHot.CLI;

namespace SupraHotEditor
{
    public partial class AvalonEditForm : Form
    {
        private TextEditor textEditor;
        private CompletionWindow completionWindow;
        private FoldingManager foldingManager;
        private object foldingStrategy;
        private string currentFileName;

        private ScriptRuntime ScriptRuntime;
        private ScriptEngine ScriptEngine;
        private ScriptScope ScriptScope;
        private ScriptOutputStream ScriptOutputStream;

        private System.Windows.Forms.TextBox ConsoleOutput;

        public AvalonEditForm()
        {
            InitializeComponent();
            textEditor = new TextEditor();
            textEditor.SetValue(TextOptions.TextFormattingModeProperty, TextFormattingMode.Display);
            textEditor.SyntaxHighlighting = HighlightingManager.Instance.GetDefinition("C#");
            textEditor.ShowLineNumbers = true;
            textEditor.FontFamily = new System.Windows.Media.FontFamily("Monaco");
            textEditor.FontSize = 14;

            ElementHost host = new ElementHost();
            host.Dock = DockStyle.Fill;
            host.Child = textEditor;

            // splitContainer1_Panel1_Paint
            splitContainer1.Panel1.Controls.Add(host);
            splitContainer1.Dock = DockStyle.Fill;

            ConsoleOutput = new System.Windows.Forms.TextBox();
            ConsoleOutput.Dock = DockStyle.Fill;
            ConsoleOutput.AutoSize = true;
            ConsoleOutput.Multiline = true;
            ConsoleOutput.ReadOnly = true;
            ConsoleOutput.ScrollBars = ScrollBars.Vertical;

            System.Drawing.Color lightGrey = System.Drawing.Color.FromArgb(40, 40, 40);
            System.Drawing.Color whiteColor = System.Drawing.Color.FromArgb(255, 255, 255);
            ConsoleOutput.BackColor = lightGrey;
            ConsoleOutput.ForeColor = whiteColor;
            splitContainer1.Panel2.BackColor = lightGrey;
            splitContainer1.Panel2.ForeColor = whiteColor;
            splitContainer1.Panel2.Controls.Add(ConsoleOutput);

            textEditor.TextArea.TextEntering += textEditor_TextArea_TextEntering;
            textEditor.TextArea.TextEntered += textEditor_TextArea_TextEntered;

            DispatcherTimer foldingUpdateTimer = new DispatcherTimer();
            foldingUpdateTimer.Interval = TimeSpan.FromSeconds(2);
            foldingUpdateTimer.Tick += delegate { UpdateFoldings(); };
            foldingUpdateTimer.Start();

            textEditor.TextArea.IndentationStrategy = new ICSharpCode.AvalonEdit.Indentation.CSharp.CSharpIndentationStrategy(textEditor.Options);
            foldingStrategy = new BraceFoldingStrategy();
            foldingManager = FoldingManager.Install(textEditor.TextArea);


            using (Stream s = Assembly.GetExecutingAssembly().GetManifestResourceStream("SupraHotEditor.python.xshd"))
            {
                using (XmlTextReader reader = new XmlTextReader(s))
                {
                    textEditor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
                }
            }

            InitIronPython();
        }

        void textEditor_TextArea_TextEntered(object sender, TextCompositionEventArgs e)
        {

            // execute script upon this line.
            DocumentLine line = textEditor.Document.GetLineByOffset(textEditor.CaretOffset);
            String currentLine = textEditor.Document.GetText(line.Offset, line.Length);
            var source = ScriptEngine.CreateScriptSourceFromString(currentLine, SourceCodeKind.Statements);

            try
            {
                source.Execute(ScriptScope);
            }
            catch(Exception exep) {}

            if (e.Text == ".")
            {
                // Get previous word
                textEditor.CaretOffset -= 1;
                String lastWord = GetLastWord(textEditor);
           //     Console.WriteLine("Last Word => {0}", lastWord);
                textEditor.CaretOffset += 1;

                // open code completion after the user has pressed dot:
                completionWindow = new CompletionWindow(textEditor.TextArea);

                // provide AvalonEdit with the data:
                IList<ICompletionData> data = completionWindow.CompletionList.CompletionData;
                
                try
                {
                    string dirCommand = "dir(" + lastWord + ")";
                    object value = ScriptScope.Engine.CreateScriptSourceFromString(dirCommand, SourceCodeKind.Expression).Execute(ScriptScope);
                    foreach (object member in (value as IronPython.Runtime.List))
                    {
                        data.Add(new MyCompletionData((string)member));
                    }
                }
                catch (Exception excep)
                {
                  //  Console.WriteLine("Exception -> {0}", excep.StackTrace);
                }


                completionWindow.Show();
                completionWindow.Closed += delegate
                {
                    completionWindow = null;
                };
            }
        }

        void textEditor_TextArea_TextEntering(object sender, TextCompositionEventArgs e)
        {
            if (e.Text.Length > 0 && completionWindow != null)
            {
                if (!char.IsLetterOrDigit(e.Text[0]))
                {
                    // Whenever a non-letter is typed while the completion window is open,
                    // insert the currently selected element.
                    completionWindow.CompletionList.RequestInsertion(e);
                }
            }
            // do not set e.Handled=true - we still want to insert the character that was typed
        }

        void UpdateFoldings()
        {
            if (foldingStrategy is BraceFoldingStrategy)
            {
                ((BraceFoldingStrategy)foldingStrategy).UpdateFoldings(foldingManager, textEditor.Document);
            }
        }

        private void AvalonEditForm_Load(object sender, EventArgs e)
        {

        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog dlg = new System.Windows.Forms.OpenFileDialog();
            dlg.CheckFileExists = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                currentFileName = dlg.FileName;
                textEditor.Load(currentFileName);               
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            if (currentFileName == null)
            {
                System.Windows.Forms.SaveFileDialog dlg = new System.Windows.Forms.SaveFileDialog();
                dlg.Filter = "python (*.py)|*.py";
                dlg.FilterIndex = 1;
                dlg.RestoreDirectory = true;
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    currentFileName = dlg.FileName;
                }
                else
                {
                    return;
                }
            }
            textEditor.Save(currentFileName);
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            textEditor.Cut();
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            textEditor.Copy();
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            textEditor.Paste();
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            textEditor.Delete();
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            textEditor.Undo();
        }

        private void toolStripButton8_Click(object sender, EventArgs e)
        {
            textEditor.Redo();
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void toolStripButton9_Click(object sender, EventArgs e)
        {
            // Play. Execute script

            String script = textEditor.Text;
            var source = ScriptEngine.CreateScriptSourceFromString(script, SourceCodeKind.Statements);

            try
            {
                source.Execute(ScriptScope);
            }
            catch (Exception ex)
            {
                using (var writer = new StreamWriter(ScriptOutputStream))
                {
                    writer.WriteLine(
                        "Oops! There was an exception while running the script:\r\n" +
                        ex.Message + "\r\n\r\n" + ex.StackTrace);
                }
            }

            if (ConsoleOutput.Text.Length > 0) 
            {
                ConsoleOutput.SelectionLength = 0;
                ConsoleOutput.SelectionStart = ConsoleOutput.Text.Length - 1;
                ConsoleOutput.ScrollToCaret();
            }
        }

        private void toolStripButton10_Click(object sender, EventArgs e)
        {
            // Clear Console
            ConsoleOutput.Text = "";
        }

        private void InitIronPython() 
        {
            ScriptEngine = Python.CreateEngine();
            ScriptScope = ScriptEngine.CreateScope();

            ScriptRuntime = ScriptEngine.Runtime;
            ScriptRuntime.LoadAssembly(typeof(String).Assembly);
            ScriptRuntime.LoadAssembly(typeof(Uri).Assembly);

            var paths = ScriptEngine.GetSearchPaths();
            paths.Add(FileSystemCLI.GetIntance().GetRootPath() + "PythonLibraries");
            ScriptEngine.SetSearchPaths(paths);

            var assemblies = AppDomain.CurrentDomain.GetAssemblies();
            foreach(var assembly in assemblies) 
            {
                ScriptRuntime.LoadAssembly(assembly);
            }

            ScriptOutputStream = new ScriptOutputStream(ConsoleOutput);
            ScriptEngine.Runtime.IO.SetOutput(ScriptOutputStream, Encoding.GetEncoding(1252));
        }

        private String GetLastWord(TextEditor editor) 
        {
            if (editor == null)
                throw new ArgumentNullException("editor");
            int endOffset = editor.CaretOffset;
            int startOffset = FindPrevWordStart(editor.Document, endOffset);
            if (startOffset < 0)
                return string.Empty;
            else
                return editor.Document.GetText(startOffset, endOffset - startOffset);
        }

        public int FindPrevWordStart(ITextSource textSource, int offset)
        {
            return TextUtilities.GetNextCaretPosition(textSource, offset, LogicalDirection.Backward, CaretPositioningMode.WordStart);
        }
    }
}
