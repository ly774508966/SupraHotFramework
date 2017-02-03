using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using SupraHot.CLI;

namespace SupraHotEditor
{
    public partial class MaterialEditForm : Form
    {
        private String MaterialDirectoryPath;
        private ListView MaterialListView;
        private TableLayoutPanel TablePanel;
        private TableLayoutPanel MaterialTablePanel;
        private Material ActiveMaterial;
        private MaterialView MaterialView;

        public MaterialEditForm(String materialDirectoryPath)
        {
            InitializeComponent();

            this.MaterialDirectoryPath = materialDirectoryPath;
            
            this.TablePanel = new TableLayoutPanel();
            this.TablePanel.Dock = DockStyle.Fill;
            this.splitContainer1.Panel1.Controls.Add(TablePanel);

            this.MaterialTablePanel = new TableLayoutPanel();
            this.MaterialTablePanel.Dock = DockStyle.Fill;
            this.splitContainer1.Panel2.Controls.Add(MaterialTablePanel);

            BuildButtons();
            BuildMaterialsList();
        }

        private void BuildButtons() 
        {
            Button createNewMaterialBtn = new Button();
            createNewMaterialBtn.Text = "New";
            TablePanel.Controls.Add(createNewMaterialBtn);

            createNewMaterialBtn.Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        NewMaterialForm newMaterialForm = new NewMaterialForm(MaterialListView);
                        newMaterialForm.ShowDialog(this);
                    }
                );
        }

        private void BuildMaterialsList() 
        {
            MaterialListView = new ListView();
            MaterialListView.Dock = DockStyle.Fill;
            MaterialListView.AutoArrange = true;
            MaterialListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
            MaterialListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            MaterialListView.Columns.Add("Content", 200, HorizontalAlignment.Left);
            MaterialListView.View = View.Details;
            TablePanel.Controls.Add(MaterialListView);
            AddFiles(MaterialDirectoryPath, MaterialListView);
          
            MaterialListView.SelectedIndexChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        var listView = (ListView)sender;

                        if (MaterialListView.SelectedItems.Count > 0) 
                        {
                            ShowMaterialProperties(MaterialListView.SelectedItems[0].Text);
                        }                        
                    }
                );
        }

        private void AddFiles(String path, ListView parent)
        {
            String[] fileEntries = Directory.GetFiles(path);
            
            foreach (String fileName in fileEntries)
            {
                var stringReplaced = fileName.Replace(path + "\\", "");
                int dotIndex = stringReplaced.LastIndexOf(".");
                stringReplaced = stringReplaced.Substring(0, dotIndex);

                Console.WriteLine("File : {0}", stringReplaced);
                MaterialListView.Items.Add(new ListViewItem(stringReplaced));
            }

            /*
             
            String[] directories = Directory.GetDirectories(path);
            foreach (String directory in directories)
            {
                var stringReplaced = directory.Replace(path + "\\", "");

                Label label = new Label();
                label.Text = stringReplaced;
                parent.Controls.Add(label);
            }
             
             */
        }

        private void ShowMaterialProperties(String materialName)
        {
            String materialFilePath = "Materials/" + materialName + ".json";

            bool fileExists = FileSystemCLI.GetIntance().FileExists(materialFilePath);

            if (fileExists)
            {
                Console.WriteLine("Material -> {0}", materialFilePath);

                // Load material.
                // Create material view and add it to the materialTable

                // Load material from cache.

                if (MaterialCacheCLI.GetIntance().IsCached(materialFilePath))
                {
                    //ActiveMaterial.Dispose();
                    ActiveMaterial = MaterialCacheCLI.GetIntance().GetCached(materialFilePath);
                }
                else
                {
                    MaterialCacheCLI.GetIntance().LoadIntoCache(materialFilePath);
                    //ActiveMaterial.Dispose();
                    ActiveMaterial = MaterialCacheCLI.GetIntance().GetCached(materialFilePath);
                }

                if (MaterialView != null)
                {
                    MaterialTablePanel.Controls.Remove(MaterialView);
                    MaterialView.Dispose();
                }

                MaterialView = new MaterialView(ActiveMaterial, materialFilePath);
                MaterialTablePanel.Controls.Add(MaterialView);
            }
            else 
            {
                if (MaterialCacheCLI.GetIntance().IsCached(materialName))
                {
                    ActiveMaterial = MaterialCacheCLI.GetIntance().GetCached(materialName);

                    if (MaterialView != null)
                    {
                        MaterialTablePanel.Controls.Remove(MaterialView);
                        MaterialView.Dispose();
                    }

                    MaterialView = new MaterialView(ActiveMaterial, materialName);
                    MaterialTablePanel.Controls.Add(MaterialView);
                }
            }

        }

        private void CreateNewMaterial(String shaderDescriptionFilePath)
        {

        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Escape)
            {
                DestroyWindowView();
              
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void MaterialEditForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            DestroyWindowView();
        }

        private void DestroyWindowView() 
        {
            if(MaterialView != null) 
            {
                MaterialView.Dispose();
            }

            if (ActiveMaterial != null)
            {
                ActiveMaterial.Dispose();
            }

            this.Dispose();
        }
    }
}
