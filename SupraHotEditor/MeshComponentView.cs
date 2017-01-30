using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using SupraHot;
using SupraHot.CLI;
using System.IO;

namespace SupraHotEditor
{
    class MeshComponentViewSeparated : ComponentView
    {
        private String SelectedMeshData = "None";
        private String SelectedMaterial;

        private ComboBox AvailableMaterials;
        private MeshComponentCLI MeshComponent;

        // Panel
        private FlowLayoutPanel groupBoxFlowLayout;

        protected override void OnPaint(PaintEventArgs pe)
        {
            int clientHeight = Form1.Instance.GetClientRectHeight() - 100;
            if(clientHeight > 700) 
            {
                clientHeight = 700;
            }
            this.Height = clientHeight;
        }

        public MeshComponentViewSeparated(MeshComponentCLI meshComponent) 
        {
            MeshComponent = meshComponent;

            this.Text = "Mesh Component View";
            this.Width = 200;
            this.Height = 400;

            Color lightGrey = Color.FromArgb(40, 40, 40);
            Color whiteColor = Color.FromArgb(255, 255, 255);
            this.BackColor = lightGrey;
            this.ForeColor = whiteColor;

            groupBoxFlowLayout = new FlowLayoutPanel();
            groupBoxFlowLayout.Dock = DockStyle.Fill;
            groupBoxFlowLayout.FlowDirection = FlowDirection.TopDown;
            groupBoxFlowLayout.WrapContents = false;
            groupBoxFlowLayout.AutoScroll = true;
            groupBoxFlowLayout.AutoSize = true;
            this.Controls.Add(groupBoxFlowLayout);

            // Title
            Label title = new Label();
            title.Text = "Mesh Component";
            groupBoxFlowLayout.Controls.Add(title);

            Label nameOfMesh = new Label();
            nameOfMesh.Text = meshComponent.GetMeshName();
            groupBoxFlowLayout.Controls.Add(nameOfMesh);

            Label labelOfModelFile = new Label();
            labelOfModelFile.Text = meshComponent.GetModelFilePath();
            groupBoxFlowLayout.Controls.Add(labelOfModelFile);

            Label labelOfModelFileArrayIndex = new Label();
            labelOfModelFileArrayIndex.Text = "[" + meshComponent.GetModelFileArrayIndex() + "]";
            groupBoxFlowLayout.Controls.Add(labelOfModelFileArrayIndex);


            // Build available Materials
            BuildMaterialsList();
            SelectCurrentMaterial();
        }

        private void ChangeMaterial()
        {
            // Get Materialdescription for SelectedMaterial and load it.
            // This should be done via a material cache, but for now it is totally fine to load it as it is.
            // This way we get a single instance of it 

            String materialFilePath = "Materials/" + SelectedMaterial + ".json";

            bool fileExists = FileSystemCLI.GetIntance().FileExists(materialFilePath);

            if (fileExists)
            {
                Console.WriteLine("Material -> {0}", materialFilePath);

                // Load material.
                // Create material view and add it to the materialTable

                GenericSerializerCLI gs = new GenericSerializerCLI(materialFilePath);
                var material = gs.DeserialzeMaterial();
                material.SetMaterialFilePath(materialFilePath);
                material.IsCopy = true;                
                gs.Dispose();

                MeshComponent.SetMaterial(material);
                AddMaterialProperties(material, materialFilePath);
                Form1.UpdateView();
            }

        }

        private void AddMaterialProperties(Material material, String materialFilePath) 
        {
            var MaterialView = new MaterialView(material, materialFilePath);
            MaterialView.Show();
        }

        private void SelectCurrentMaterial() 
        {
            String materialName = MeshComponent.GetMaterial().GetName();
            int count = 0;
            foreach(String material in AvailableMaterials.Items)
            {
                if (materialName == material) 
                {
                    AvailableMaterials.SelectedIndex = count;
                    ChangeMaterial();
                }
                count++;
            }
        }

        private void BuildMaterialsList()
        {
            AvailableMaterials = new ComboBox();
            AvailableMaterials.DropDownStyle = ComboBoxStyle.DropDownList;

            AvailableMaterials.SelectedIndexChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        ComboBox comboBox = (ComboBox)sender;
                        string selectedProperty = (string)comboBox.SelectedItem;

                        if (selectedProperty != SelectedMaterial)
                        {
                            Console.WriteLine("Selected {0}", selectedProperty);
                            SelectedMaterial = selectedProperty;
                            ChangeMaterial();
                        }
                    }
                );


            groupBoxFlowLayout.Controls.Add(AvailableMaterials);
            AddFiles(FileSystemCLI.GetIntance().GetRootPath() + "Materials");
        }

        private void AddFiles(String path)
        {
            String[] fileEntries = Directory.GetFiles(path);

            foreach (String fileName in fileEntries)
            {
                var stringReplaced = fileName.Replace(path + "\\", "");
                int dotIndex = stringReplaced.LastIndexOf(".");
                stringReplaced = stringReplaced.Substring(0, dotIndex);

                Console.WriteLine("File : {0}", stringReplaced);
                AvailableMaterials.Items.Add(stringReplaced);
            }
        }
    }
}
