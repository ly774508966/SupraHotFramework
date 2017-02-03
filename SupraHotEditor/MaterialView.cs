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
    class MaterialView : ComponentView
    {
        private Material ActiveMaterial;
        private String MaterialFilePath;
        private String SelectedShader;

        private ComboBox AvailableMaterialPropertiesComboBox;
        private Dictionary<String, String> AvailableMaterialProperties;
        private List<MaterialPropertyWidget> ActiveMaterialProperties;

        private List<MaterialStatePair> CurrentMaterialState;

        private TableLayoutPanel GroupTablePanel;

        public MaterialView(Material materialCLI, String materialFilePath)
        {
            // Set style
            this.Dock = DockStyle.Fill;
            GroupTablePanel = new TableLayoutPanel();
            GroupTablePanel.Dock = DockStyle.Fill;
            GroupTablePanel.AutoScroll = true;
            GroupTablePanel.AutoSize = true;
            Color lightGrey = Color.FromArgb(40, 40, 40);
            Color whiteColor = Color.FromArgb(255, 255, 255);
            GroupTablePanel.BackColor = lightGrey;
            GroupTablePanel.ForeColor = whiteColor;
            this.Controls.Add(GroupTablePanel);

            // Assign material
            MaterialFilePath = materialFilePath;
            ActiveMaterial = materialCLI;
            AvailableMaterialProperties = new Dictionary<String, String>();
            List<MaterialPropertyCommonInterface> copiedMaterialProperties = ActiveMaterial.GetMaterialProperties();
            SelectedShader = ActiveMaterial.GetShaderDescriptionName();
            Console.WriteLine("Shader name = {0}", ActiveMaterial.GetShaderDescriptionName());
            Console.WriteLine("COUNT OF MATERIAL PROPERTIES = {0}", copiedMaterialProperties.Count);
            SelectShader(SelectedShader);

            // GUI stuff
            Label materialNameLabel = new Label();
            materialNameLabel.Width = 350;
            materialNameLabel.Text = "Material name: " + ActiveMaterial.GetName();
            GroupTablePanel.Controls.Add(materialNameLabel);

            Label selectedShaderLabel = new Label();
            selectedShaderLabel.Width = 350;
            selectedShaderLabel.Text = "Base shader: " + SelectedShader;
            GroupTablePanel.Controls.Add(selectedShaderLabel);

            Button saveMaterialButton = new Button();
            saveMaterialButton.Text = "Save";
            saveMaterialButton.Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        SaveMaterial();
                    }
                );
            GroupTablePanel.Controls.Add(saveMaterialButton);

            BuildAvailableMaterialPropertiesList();

            // Copy over the already set material properties on this material to a list
            if (copiedMaterialProperties.Count > 0)
            {
                foreach (MaterialPropertyCommonInterface mpci in copiedMaterialProperties)
                {
                    MaterialPropertyWidget mpw = new MaterialPropertyWidget(mpci, this);
                    GroupTablePanel.Controls.Add(mpw);
                    ActiveMaterialProperties.Add(mpw);
                    AvailableMaterialPropertiesComboBox.Items.Remove(mpci.GetName());
                }
            }

            // Build material state as of it is when loaded from disk
            // We need this to check, if the material was changed, but not saved.
            // If this is the case, we will revert the material to it's "when-loaded state"
            BuildCurrentMaterialState();
        }

        private void BuildAvailableMaterialPropertiesList()
        {
            if (AvailableMaterialPropertiesComboBox != null)
            {
                GroupTablePanel.Controls.Remove(AvailableMaterialPropertiesComboBox);

                foreach (MaterialPropertyWidget widget in ActiveMaterialProperties)
                {
                    GroupTablePanel.Controls.Remove(widget);
                }

                ActiveMaterialProperties.Clear();
            }

            ActiveMaterialProperties = new List<MaterialPropertyWidget>();
            AvailableMaterialPropertiesComboBox = new ComboBox();
            AvailableMaterialPropertiesComboBox.Sorted = true;
            AvailableMaterialPropertiesComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            GroupTablePanel.Controls.Add(AvailableMaterialPropertiesComboBox);


            AvailableMaterialPropertiesComboBox.Items.Add("Add property");
            AvailableMaterialPropertiesComboBox.SelectedIndex = 0;

            foreach (KeyValuePair<string, string> materialProperty in AvailableMaterialProperties)
            {
                AvailableMaterialPropertiesComboBox.Items.Add(materialProperty.Key);
            }

            // Add an EventListener to the ComboBox!
            AvailableMaterialPropertiesComboBox.SelectedIndexChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        ComboBox comboBox = (ComboBox)sender;

                        if (comboBox.SelectedIndex > 0)
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

        private void AddMaterialProperty(String name, String type)
        {
            // Add native material property to material
            Console.WriteLine("CLI::AddMaterialProperty {0} {1}", name, type);
            var materialPropertyCLI = ActiveMaterial.AddMaterialProperty(name, type);

            if (materialPropertyCLI != null)
            {
                Console.WriteLine("Add Widget!");
                MaterialPropertyWidget mpw = new MaterialPropertyWidget(materialPropertyCLI, this);
                GroupTablePanel.Controls.Add(mpw);
                ActiveMaterialProperties.Add(mpw);
            }
        }

        public void RemoveMaterialProperty(String name, MaterialPropertyCommonInterface materialPropertyCommonInterface, MaterialPropertyWidget materialPropertyWidget)
        {
            Console.WriteLine("Remove {0}", name);
            AvailableMaterialPropertiesComboBox.Items.Add(name);
            GroupTablePanel.Controls.Remove(materialPropertyWidget);
            ActiveMaterialProperties.Remove(materialPropertyWidget);
            ActiveMaterial.RemoveMaterialProperty(name);
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
                switch (type)
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
        }

        private void SaveMaterial()
        {
            if (ActiveMaterial != null)
            {
                GenericSerializerCLI gs = new GenericSerializerCLI(MaterialFilePath);
                gs.Serialize(ActiveMaterial);
                gs.Dispose();
                Console.WriteLine("Saved Material.");

                // Put all material properties and their values inside a list
                // On exit, we will set the material's properties back to this list.
                // So, if the user modified the material, but did not save it, the material
                // will be reset to it's previous state.
                BuildCurrentMaterialState();
            }
        }

        public void Dispose()
        {
            if (ActiveMaterial != null)
            {
                ResetMaterialIfModifiedButNotSaved();
                ActiveMaterial.Dispose();                
            }

            // Dispose of unmanaged resources.
            Dispose(true);

            // Suppress finalization.
            GC.SuppressFinalize(this);
        }

        // Todo: Hook into window closign function

        private void ResetMaterialIfModifiedButNotSaved()
        {
            Console.WriteLine("Material View Disposed -> {0}", ActiveMaterial);

            // Check if current changes to the material were saved,
            // if not revert them.

            ActiveMaterial.RemoveAllMaterialProperties();
            foreach(MaterialStatePair pair in CurrentMaterialState) 
            {
                var materialPropertyInterface = ActiveMaterial.AddMaterialProperty(pair.Name, pair.Type);
                var type = materialPropertyInterface.GetType();

                if (type == "Boolean")
                {
                    ((BooleanMaterialPropertyCLI)materialPropertyInterface).SetValue((bool)pair.Value);
                }
                else if (type == "Float")
                {
                    ((FloatMaterialPropertyCLI)materialPropertyInterface).SetValue((float)pair.Value);
                }
                else if (type == "Vec2")
                {
                    ((Vec2MaterialPropertyCLI)materialPropertyInterface).SetValue((Vec2CLI)pair.Value);
                }
                else if (type == "Vec3")
                {
                    ((Vec3MaterialPropertyCLI)materialPropertyInterface).SetValue((Vec3CLI)pair.Value);
                }
                else if (type == "Vec4")
                {
                    ((Vec4MaterialPropertyCLI)materialPropertyInterface).SetValue((Vec4CLI)pair.Value);
                }
                else if (type == "Texture2D")
                {
                    ((Texture2DMaterialPropertyCLI)materialPropertyInterface).SetValue((String)pair.Value);
                }
                else if (type == "TextureCube")
                {
                    ((TextureCubeMaterialPropertyCLI)materialPropertyInterface).SetValue((String)pair.Value);
                }


                Console.WriteLine("Reset: {0} -> {1} ", pair.Name, pair.Value.ToString());
            }

            Form1.UpdateView();
        }

        private void BuildCurrentMaterialState()
        {
            CurrentMaterialState = new List<MaterialStatePair>();
            foreach (MaterialPropertyWidget mpw in ActiveMaterialProperties)
            {
                MaterialStatePair msp = new MaterialStatePair();
                msp.Name  = mpw.GetName();
                msp.Type  = mpw.GetType();
                msp.Value = GetValueFromMaterialPropertyWidget(mpw);

                CurrentMaterialState.Add(msp);
            }
        }

        private Object GetValueFromMaterialPropertyWidget(MaterialPropertyWidget mpw) 
        {
            var type = mpw.GetType();

            if (type == "Boolean") 
            {
                return ((BooleanMaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "Float") 
            {
                return ((FloatMaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "Vec2") 
            {
                return ((Vec2MaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "Vec3") 
            {
                return ((Vec3MaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "Vec4") 
            {
                return ((Vec4MaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "Texture2D") 
            {
                return ((Texture2DMaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
            else if (type == "TextureCube") 
            {
                return ((TextureCubeMaterialPropertyCLI)mpw.GetMaterialPropertyCommonInterface()).GetValue();
            }
           
            return null;
        }
    }

    class MaterialStatePair 
    {
        public String Name;
        public String Type;
        public Object Value;
    };
}
