using System;
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
    class MeshComponentView : ComponentView
    {
        private String SelectedMeshData = "None";
        private String SelectedShader;
        private Dictionary<String, String> AvailableMaterialProperties;
        private List<MaterialPropertyWidget> ActiveMaterialProperties;

        private ComboBox ShaderNameComboBox;
        private ComboBox AvailableMaterialPropertiesComboBox;

        private Material Material;
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

        public MeshComponentView(Material material, MeshComponentCLI meshComponent) 
        {
            Console.WriteLine("MeshComponentView Constructor for {0}", material.GetName());

            List<MaterialPropertyCommonInterface> copiedMaterialProperties = material.GetMaterialProperties();

            this.Material = material;
            this.MeshComponent = meshComponent;

            AvailableMaterialProperties = new Dictionary<String, String>();

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


            // Drop down for ShaderDescriptions

            Label selectShader = new Label();
            selectShader.Text = "Selected shader:";
            groupBoxFlowLayout.Controls.Add(selectShader);

            ShaderNameComboBox = new ComboBox();
            groupBoxFlowLayout.Controls.Add(ShaderNameComboBox);
            ShaderNameComboBox.DropDownStyle = ComboBoxStyle.DropDownList;



            // - - - - - - - - - -
            // NEEDED CODE
            // - - - - - - - - - -

            // Init the Component's view data
            ShaderLibraryCLI shaderLibrary = ShaderLibraryCLI.GetIntance();
            var shaderNames = shaderLibrary.GetAvailableShaderNames();

            foreach(String shaderName in shaderNames) 
            {
                // Build drop down for shaders
                ShaderNameComboBox.Items.Add(shaderName);
            }

            if(material.GetShaderDescriptionName() != null) 
            {
                for (int i = 0; i < shaderNames.Count; i++ )
                {
                    if (shaderNames[i] == material.GetShaderDescriptionName()) 
                    {
                       BuildData(shaderNames[i], copiedMaterialProperties.Count <= 0);
                       ShaderNameComboBox.SelectedIndex = i;


                       Console.WriteLine("Material properties count after shader build -> {0}", copiedMaterialProperties.Count);
                       if (copiedMaterialProperties.Count > 0)
                       {
                           // todo: also check, if we have set a shader on this material.
                           // if we did, then load it up

                           // add mps to this view comp.

                           foreach (MaterialPropertyCommonInterface mpci in copiedMaterialProperties)
                           {
                               MaterialPropertyWidget mpw = new MaterialPropertyWidget(mpci, this);
                               groupBoxFlowLayout.Controls.Add(mpw);
                               ActiveMaterialProperties.Add(mpw);
                               AvailableMaterialPropertiesComboBox.Items.Remove(mpci.GetName());
                           }

                           this.MeshComponent.UpdateShaderPermuation();
                           Form1.UpdateView();
                       }

                    }
                    else 
                    {
                        // need to load the missing shader.
                    }
                }
            }
            else 
            {
               if(shaderNames.Count > 0) 
                {
                    BuildData(shaderNames[0], true);
                    ShaderNameComboBox.SelectedIndex = 0;
                }
            }

            ShaderNameComboBox.SelectedIndexChanged += new EventHandler(
                delegate(object sender, EventArgs e)
                {
                    ComboBox comboBox = (ComboBox)sender;
                    string selectedShader = (string)comboBox.SelectedItem;

                    if (selectedShader != SelectedShader) 
                    {
                        BuildData(selectedShader, true);
                    }
                }
            );
        }

        private void SelectShader(String selectedShader) 
        {
            Console.WriteLine("Selected Shader: {0}", selectedShader);

            List<ShaderUniformCLI> availableMaterialProperties = ShaderLibraryCLI.GetIntance().GetAvailableShaderUniforms(selectedShader);
            AvailableMaterialProperties.Clear();

            foreach (ShaderUniformCLI su in availableMaterialProperties) 
            {
                String name = su.Name;
                String type = su.Type;

                Console.WriteLine("{0} -> {1}", su.Name, su.Type);

                // We only want Uniforms from the list below to be available.
                // We don't care about matrices as of now!
                // HINT: Here we just save the uniforms from the shaders, except mat3 and mat4.
                // The actuall "parsing" is done in the MaterialCLI-Class
                switch(type)
                {
                    case "bool":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "float":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "vec2":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "vec3":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "vec4":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "sampler2D":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    case "samplerCube":
                        AvailableMaterialProperties.Add(name, type);
                        break;
                    default: 
                        break;
                }
            }

            this.MeshComponent.UpdateShaderPermuation();

        }

        // Gui Method.
        private void BuildAvailableMaterialPropertiesList() 
        {
            if (AvailableMaterialPropertiesComboBox != null) 
            {
                groupBoxFlowLayout.Controls.Remove(AvailableMaterialPropertiesComboBox);

                foreach(MaterialPropertyWidget widget in ActiveMaterialProperties) 
                {
                    groupBoxFlowLayout.Controls.Remove(widget);
                }

                ActiveMaterialProperties.Clear();
            }

            ActiveMaterialProperties = new List<MaterialPropertyWidget>();
            AvailableMaterialPropertiesComboBox = new ComboBox();
            AvailableMaterialPropertiesComboBox.Sorted = true;
            AvailableMaterialPropertiesComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            groupBoxFlowLayout.Controls.Add(AvailableMaterialPropertiesComboBox);

 
            AvailableMaterialPropertiesComboBox.Items.Add("Add property");
            AvailableMaterialPropertiesComboBox.SelectedIndex = 0;

            foreach(KeyValuePair<string, string> materialProperty in AvailableMaterialProperties) 
            {
                AvailableMaterialPropertiesComboBox.Items.Add(materialProperty.Key);
            }

            // Add an EventListener to the ComboBox!
            AvailableMaterialPropertiesComboBox.SelectedIndexChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        ComboBox comboBox = (ComboBox)sender;

                        if(comboBox.SelectedIndex > 0) 
                        {
                            string selectedProperty = (string)comboBox.SelectedItem;
                            comboBox.SelectedIndex = 0;

                            Console.WriteLine("Selected {0}", selectedProperty);

                            // Remove the selected Entry
                            comboBox.Items.Remove(selectedProperty);
                            AddMaterialProperty(selectedProperty, AvailableMaterialProperties[selectedProperty]);
                        }
                    }
                );

        }

        // Gui method.
        private void AddMaterialProperty(String name, String type) 
        {
            // Add native material property to material
            Console.WriteLine("CLI::AddMaterialProperty {0} {1}", name, type);
            var materialPropertyCLI = this.Material.AddMaterialProperty(name, type);
            this.MeshComponent.UpdateShaderPermuation();

            if (materialPropertyCLI != null) 
            {
                Console.WriteLine("Add Widget!");
                groupBoxFlowLayout.FlowDirection = FlowDirection.TopDown;
                MaterialPropertyWidget mpw = new MaterialPropertyWidget(materialPropertyCLI, this);
                groupBoxFlowLayout.Controls.Add(mpw);
                ActiveMaterialProperties.Add(mpw);
                Form1.UpdateView();
            }
        }

        public void RemoveMaterialProperty(String name, MaterialPropertyCommonInterface materialPropertyCommonInterface, MaterialPropertyWidget materialPropertyWidget) 
        {
            Console.WriteLine("Remove {0}", name);

            AvailableMaterialPropertiesComboBox.Items.Add(name);

            groupBoxFlowLayout.Controls.Remove(materialPropertyWidget);
            ActiveMaterialProperties.Remove(materialPropertyWidget);
            this.Material.RemoveMaterialProperty(name); // This removes it by name!

            this.MeshComponent.UpdateShaderPermuation();
            Form1.UpdateView();
        }

        private bool SwitchNativeShader() 
        {
            String shaderName = this.SelectedShader;
            return this.MeshComponent.SetShader(shaderName);
        }

        private void BuildData(String selectedShader, bool switchNativeShader)
        {
            this.SelectedShader = selectedShader;

            if (switchNativeShader) 
            {
                SwitchNativeShader();
            }

            SelectShader(selectedShader);
            BuildAvailableMaterialPropertiesList();

            Form1.UpdateView();
        }
    }
}
