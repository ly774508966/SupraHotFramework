using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using SupraHot;
using SupraHot.CLI;

namespace SupraHotEditor
{
    class AssetBrowserView : Panel
    {
        private String CurrentDirectory;
        private Label CurrentDirectoryLabel;
        private FlowLayoutPanel LayoutPanel;
        
        public AssetBrowserView() 
        {
            Color clr = Color.FromArgb(100, 100, 100);
            this.BackColor = clr;

            this.Dock = DockStyle.Fill;

            CurrentDirectoryLabel = new Label();
            CurrentDirectoryLabel.Text = "";

            LayoutPanel = new FlowLayoutPanel();
            LayoutPanel.Dock = DockStyle.Fill;
            
            this.Controls.Add(LayoutPanel);
        }

        public void SetDirectory(String directory) 
        {
            CurrentDirectory = directory;
            CurrentDirectoryLabel.Text = directory;

            // Todo: Remove already set FileSystemWatcher, if we have one!
            // Todo: Set up FileSystemWatcher on this given directory

            LayoutPanel.Controls.Clear();
            LayoutPanel.Controls.Add(CurrentDirectoryLabel);

            String fileSystemRoot = FileSystemCLI.GetIntance().GetRootPath();
            AddFiles(fileSystemRoot + directory, LayoutPanel);
            BuildView();
        }

        private void BuildView() 
        {
            Console.WriteLine(" Build View for {0} ", this.CurrentDirectory);
        }

        private void AddFiles(String path, Panel parent)
        {
            String[] fileEntries = Directory.GetFiles(path);

            foreach (String fileName in fileEntries)
            {
                var stringReplaced = fileName.Replace(path + "\\", "");
                Console.WriteLine("File : {0}", stringReplaced);

                Label label = new Label();
                label.Text = stringReplaced;
                parent.Controls.Add(label);
            }

            String[] directories = Directory.GetDirectories(path);

            foreach (String directory in directories)
            {
                var stringReplaced = directory.Replace(path + "\\", "");

                Label label = new Label();
                label.Text = stringReplaced;
                parent.Controls.Add(label);
                //AddFiles(directory, parent);

                label.DoubleClick += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        var origin = (Label)sender;
                        String folder = CurrentDirectory + "/" + origin.Text;
                        Console.WriteLine("Clicked on {0}", folder);
                        SetDirectory(folder);
                    }
                );
            }
        }

        private Panel CreateAssetIcon(String pathToAsset) 
        {
            return null;
        }

        private Panel CreateFolderIcon(String pathToDirectory) 
        {
            return null;
        }
    }
}
