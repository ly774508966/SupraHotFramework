using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SupraHot.Graphics;

namespace SupraHotEditor
{
  
    public partial class Form1 : Form
    {
        public static WindowEditor SupraHotWindow;
        public static AppEditor appEdit;

        public Form1()
        {
            InitializeComponent();

            unsafe
            {
                void* hwndPointer = splitContainer2.Panel2.Handle.ToPointer();
                SupraHotWindow = new WindowEditor();
                SupraHotWindow.SetHWND(hwndPointer);
                SupraHotWindow.Init(0, 0);
                SupraHotWindow.SetClearColor(1, 0, 0, 1);

                appEdit = new AppEditor();
                appEdit.Init(SupraHotWindow);
            }
            
        }

        private void UpdateView() 
        {
            Random r = new Random();
            SupraHotWindow.SetClearColor(r.Next(0, 100) / 100.0f, r.Next(0, 100) / 100.0f, r.Next(0, 100) / 100.0f, 1.0f);
            appEdit.ProgressApp();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
           Console.WriteLine("Paint");
           UpdateView();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Console.WriteLine("Button clicked!");
        }

        private void splitContainer2_Panel2_Paint(object sender, PaintEventArgs e)
        {
            UpdateView();
        }

        private void mainSplitContainer_Panel1_Paint(object sender, PaintEventArgs e)
        {
            UpdateView();
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {
            UpdateView();
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {
            UpdateView();
        }

        private void splitContainer2_Panel2_Paint_1(object sender, PaintEventArgs e)
        {
            UpdateView();
        }

        private void createToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.WriteLine("Create new entity");
        }

        private void importModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.WriteLine("Import model");
            openFileDialog.Filter = "shfm|*.shfm";

            if (openFileDialog.ShowDialog() == DialogResult.OK) 
            {
                String fileName = openFileDialog.SafeFileName;
                String filePath = openFileDialog.FileName;
                Console.WriteLine("Filename {0} at {1}", fileName, filePath);
            }
        }

        private void importShaderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.WriteLine("import shader");
        }

        private void importMaterialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.WriteLine("import material");
        }

    }
}
