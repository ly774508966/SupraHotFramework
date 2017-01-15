using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using SupraHot;
using SupraHot.Graphics;
using SupraHot.CLI;

namespace SupraHotEditor
{
  
    public partial class Form1 : Form
    {
        public static WindowEditor SupraHotWindow;
        public static AppEditor appEdit;
        public static Form1 Instance;

        public bool MouseRightDown = false;
        public bool MouseMiddleDown = false;
        public Point PreviousPosition;

        private EntityCLI rootEntity;

        private Panel EntityHierarchy;
        private FlowLayoutPanel ComponentPanel;

        public int GetClientRectHeight() 
        {
            return ClientRectangle.Height;
        }

        public Form1()
        {
            Instance = this;
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

                // - - - - - - - - - - - - - - - - - 
                // - - - - Entity Hierarchy - - - - -
                // - - - - - - - - - - - - - - - - - 

                CreateEntityHierarchyPanel();

                // - - - - - - - - - - - - - - - - - -
                // - - - -  Init Assets View  - - - - 
                // - - - - - - - - - - - - - - - - - -
                var assetViewParent = splitContainer3;
                var assetBrowserViewContainer = assetViewParent.Panel2;

                var assetListViewContainer = assetViewParent.Panel1;
                Color clr = Color.FromArgb(70, 70, 70);
                assetListViewContainer.BackColor = clr;

                String[] watchDirectories = new String[] 
                { 
                    "Textures",
                    "Models",
                    "Materials",
                    "Shaders",
                    "Scripts",
                    "EnvMaps",
                    "Prefabs"
                };

                var assetListView = new ListView();
                assetListView.AutoArrange = true;
                assetListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
                assetListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
                assetListView.Columns.Add("Content", 100, HorizontalAlignment.Left);
                assetListView.View = View.Details;
                assetListView.Dock = DockStyle.Fill;

                assetListViewContainer.Controls.Add(assetListView);
        
                foreach(String directory in watchDirectories) 
                {
                    assetListView.Items.Add(new ListViewItem(directory));
                }

                // Create custom ListView

                var assetBrowserView = new AssetBrowserView();
                assetBrowserViewContainer.Controls.Add(assetBrowserView);
                assetBrowserView.SetDirectory(watchDirectories[0]);

                assetListView.SelectedIndexChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        var listView = (ListView)sender;

                        if (listView.SelectedItems.Count > 0) 
                        {
                            Console.WriteLine("AssetListView active item: {0}", listView.SelectedItems[0].Text);
                            assetBrowserView.SetDirectory(listView.SelectedItems[0].Text);
                        }
                    }
                );



                // - - - - - - - - - - - - - - - - -
                // - - - - Properties View - - - - -
                // - - - - - - - - - - - - - - - - -
                rootEntity = new EntityCLI();
                rootEntity.SetName("Scene");

                // Debug
                // Load a model 
                MeshLoaderCLI meshLoader = MeshLoaderCLI.GetIntance();

                EntityCLI loadedEntity = meshLoader.LoadSFHM("Models/Pistol/Pistol_Model.shfm");
                loadedEntity.IsCopy = true;
                rootEntity.AddChild(loadedEntity);
                

                RebuildEntityHierarchy();
            }
            
        }

        private void ShowComponents(EntityCLI entity) 
        {
            if (ComponentPanel != null) 
            {
                mainSplitContainer.Panel2.Controls.Remove(ComponentPanel);
            }

            ComponentPanel = new FlowLayoutPanel();
            ComponentPanel.Dock = DockStyle.Fill;
            ComponentPanel.FlowDirection = FlowDirection.TopDown;
            ComponentPanel.WrapContents = false;
            ComponentPanel.AutoScroll = true;
            ComponentPanel.AutoSize = true;

            mainSplitContainer.Panel2.Controls.Add(ComponentPanel);

            Label entityName = new Label();
            entityName.Text = entity.GetName();
            ComponentPanel.Controls.Add(entityName);

            // Transform component
            ComponentPanel.Controls.Add(new TransformComponentView(new TransformComponentCLI(entity)));

            MeshComponentCLI meshComponent = entity.GetComponent<MeshComponentCLI>();
            if (meshComponent != null) 
            {
                Material mat = meshComponent.GetMaterial();
                MeshComponentView meshComponentView = new MeshComponentView(mat, meshComponent);
                ComponentPanel.Controls.Add(meshComponentView);    
            }
        }

        private void CreateEntityHierarchyPanel() 
        {
            if (EntityHierarchy != null)        
            {
                splitContainer2.Panel1.Controls.Remove(EntityHierarchy);
            }

            EntityHierarchy = new Panel();
            EntityHierarchy.Dock = DockStyle.Fill;
            Color entityHierarchyBackgroundColor = Color.FromArgb(90, 90, 90);
            EntityHierarchy.BackColor = entityHierarchyBackgroundColor;
            splitContainer2.Panel1.Controls.Add(EntityHierarchy);
        }

        private void LoadModelFile(String pathToModelFile)
        {
            MeshLoaderCLI meshLoader = MeshLoaderCLI.GetIntance();

            EntityCLI loaded = meshLoader.LoadSFHM(pathToModelFile);
            loaded.IsCopy = true;
            rootEntity.AddChild(loaded);
            RebuildEntityHierarchy();
            UpdateView();
        }

        private void AddTreeNodesToParent(TreeNode parent, EntityCLI entity) 
        {
            List<EntityCLI> children = entity.GetChildren();
            TreeNode entityNode = new TreeNode(entity.GetName());
            parent.Nodes.Add(entityNode);

            if(children.Count > 0) 
            {
                foreach(EntityCLI child in children) 
                {
                    AddTreeNodesToParent(entityNode, child);
                }
            }
        }

        private void TriggerNodeMouseClick(String text, EntityCLI entity) 
        {
            // Cheap hack. We need a hash map for that
            if (entity.GetName() == text)
            {
                ShowComponents(entity);
                return;
            }

            var children = entity.GetChildren();
            if (children.Count > 0)
            {
                foreach(EntityCLI child in children) 
                {
                    TriggerNodeMouseClick(text, child);
                }
            }
        }

        private void RebuildEntityHierarchy()
        {
            Console.WriteLine("Rebuild.");
            CreateEntityHierarchyPanel();
            FlowLayoutPanel flowLayout = new FlowLayoutPanel();
            flowLayout.Dock = DockStyle.Fill;
            flowLayout.FlowDirection = FlowDirection.TopDown;
            flowLayout.WrapContents = false;
            //EntityHierarchy.Controls.Add(flowLayout);

            // Link entities to parent.
            TreeView treeView = new TreeView();
            treeView.Dock = DockStyle.Fill;

            TreeNode rootNode = new TreeNode(rootEntity.GetName());
            treeView.Nodes.Add(rootNode);

            List<EntityCLI> children = rootEntity.GetChildren();

            foreach (EntityCLI entity in children)
            {
                AddTreeNodesToParent(rootNode, entity);
            }

            EntityHierarchy.Controls.Add(treeView);

            treeView.NodeMouseClick += new TreeNodeMouseClickEventHandler(
                delegate(object sender, TreeNodeMouseClickEventArgs e)
                {
                    Console.WriteLine("Index {0}", e.Node.Index);
                    TriggerNodeMouseClick(e.Node.Text, rootEntity);
                }
            );

        }

        public static void UpdateView() 
        {
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

            GenericSerializerCLI gs = new GenericSerializerCLI("Dev/Wurst.json");
            gs.Serialize(rootEntity);
            // Serialize entites[0]
        }

        private void importModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.WriteLine("Import model");
            openFileDialog.Filter = "shfm|*.shfm";

            if (openFileDialog.ShowDialog() == DialogResult.OK) 
            {
                String fileName = openFileDialog.SafeFileName;
                String filePath = openFileDialog.FileName;

                // Check if model is in Content.
                String pathToFile = FileSystemUtil.IsInContent(filePath);
                if(pathToFile != null) 
                {
                    Console.WriteLine("PathToFile {0}", pathToFile);
                    LoadModelFile(pathToFile);
                }
                else
                {
                    Console.WriteLine("{0} is not in the Content/ directory. Please move it there", fileName);
                }
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

        private void splitContainer2_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer1_Panel2_Paint_1(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer3_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer3_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer2_Panel1_Enter(object sender, EventArgs e)
        {
            
        }

        private void splitContainer2_Panel1_MouseEnter(object sender, EventArgs e)
        {
            ((Panel)sender).Focus();
        }

    }
}
