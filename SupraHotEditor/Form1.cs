using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using SupraHot.Graphics;
using SupraHot.CLI;

namespace SupraHotEditor
{
  
    public partial class Form1 : Form
    {
        public static WindowEditor SupraHotWindow;
        public static AppEditor appEdit;

        public bool MouseRightDown = false;
        public bool MouseMiddleDown = false;
        public Point PreviousPosition;

        public Form1()
        {
            InitializeComponent();
            this.splitContainer2.Panel2.MouseWheel += splitContainer2_Panel_MouseWheel;

            unsafe
            {
                // put this inside panel2_Init
                void* hwndPointer = splitContainer2.Panel2.Handle.ToPointer();
                SupraHotWindow = new WindowEditor();
                SupraHotWindow.SetHWND(hwndPointer);
                SupraHotWindow.Init(1024, 768);
                SupraHotWindow.SetClearColor(0, 0, 0, 0);

                appEdit = new AppEditor();
                appEdit.Init(SupraHotWindow);

                // Test some stuff
                var entity = new EntityCLI();
                appEdit.AddEntity(entity);
                appEdit.RemoveEntity(entity);

                var e = new EntityCLI();
                Material material = new Material();
                MeshData meshData = new MeshData();
                var c = new MeshComponent(meshData, material);
                e.AddComponent(c);

                MeshLoaderCLI meshLoader = MeshLoaderCLI.GetIntance();
                meshLoader.LoadSFHM("Models/Sponza/Sponza_M.shfm");
             
                // Test a thing here
                FlowLayoutPanel groupBoxFlowLayout = new FlowLayoutPanel();
                groupBoxFlowLayout.Dock = DockStyle.Fill;
                mainSplitContainer.Panel2.Controls.Add(groupBoxFlowLayout);

                MaterialPropertyWidget bmpc = new MaterialPropertyWidget(new BooleanMaterialPropertyCLI("Bool value"));
                groupBoxFlowLayout.Controls.Add(bmpc);

                MaterialPropertyWidget v3mpc = new MaterialPropertyWidget(new Vec3MaterialPropertyCLI("Color"));
                groupBoxFlowLayout.Controls.Add(v3mpc);
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

        private void Form1_Load(object sender, EventArgs e)
        {
            Application.ApplicationExit += new EventHandler(OnExit);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void OnExit(object sender, EventArgs e)
        {
            appEdit.Destroy();
        }

        private void splitContainer2_Panel2_Resize(object sender, EventArgs e)
        {
            var panel = ((Panel)sender);
            if(appEdit != null) 
            {
                appEdit.Resize((uint)panel.Width, (uint)panel.Height);
            }
        }

        private void splitContainer2_Panel2_MouseDown(object sender, MouseEventArgs e)
        {
            // make this the right mbutton

            // mouse down
            MouseRightDown = e.Button == MouseButtons.Right;
            MouseMiddleDown = e.Button == MouseButtons.Middle;
        }

        private void splitContainer2_Panel2_MouseUp(object sender, MouseEventArgs e)
        {
            // mouse up
            MouseRightDown = false;
            MouseMiddleDown = false;
        }

        private void splitContainer2_Panel2_MouseMove(object sender, MouseEventArgs e)
        {
            Point mousePos = MousePosition;

            // mouse move
            if (MouseRightDown && appEdit != null) 
            {
                float deltaX = PreviousPosition.X - mousePos.X;
                float deltaY = PreviousPosition.Y - mousePos.Y;

                // - rotate
                CameraCLI camera = appEdit.GetCamera();
                camera.SetYaw(camera.GetYaw() - (deltaX * 0.16f));
                camera.SetPitch(camera.GetPitch() - (deltaY * 0.16f));


                UpdateView();
            } 
            else if (MouseMiddleDown && appEdit != null) 
            {
                float deltaX = PreviousPosition.X - mousePos.X;
                float deltaY = PreviousPosition.Y - mousePos.Y;

                // move right, left, up down
                CameraCLI camera = appEdit.GetCamera();
                camera.MoveRight(-deltaX * 0.16f);
                camera.MoveUp(deltaY * 0.16f);

                UpdateView();
            }

            PreviousPosition = mousePos;
        }

        private void splitContainer2_Panel_MouseWheel(object sender, MouseEventArgs e) 
        {
            CameraCLI camera = appEdit.GetCamera();
            camera.MoveForward((e.Delta > 0 ? 15 : -15) * 0.16f);
            UpdateView();
        }

        private void splitContainer2_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            Console.WriteLine("Scroll");
        }

        private void splitContainer2_Panel2_MouseEnter(object sender, EventArgs e)
        {
            this.splitContainer2.Panel2.Focus();
        }

        private void splitContainer2_Panel2_MouseLeave(object sender, EventArgs e)
        {
            this.Focus();
        }

    }
}
