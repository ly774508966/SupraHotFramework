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
using System.Text.RegularExpressions;
using SupraHot.CLI;

namespace SupraHotEditor
{
    public partial class NewMaterialForm : Form
    {
        private TableLayoutPanel TablePanel;
        private ListView MaterialListView;
        private ComboBox AvailableShaders;
        private TextBox MaterialNameTextBox;

        public NewMaterialForm(ListView parentListView)
        {
            InitializeComponent();

            this.MaterialListView = parentListView;

            this.TablePanel = new TableLayoutPanel();
            this.TablePanel.Dock = DockStyle.Fill;
            this.Controls.Add(TablePanel);

            Label materialNameLabel = new Label();
            materialNameLabel.Text = "Material Name";
            this.TablePanel.Controls.Add(materialNameLabel);

            MaterialNameTextBox = new TextBox();
            MaterialNameTextBox.Width = this.Width - 25;
            this.TablePanel.Controls.Add(MaterialNameTextBox);


            //...

            AvailableShaders = new ComboBox();
            AvailableShaders.Width = this.Width - 25;
            AvailableShaders.Sorted = true;
            AvailableShaders.DropDownStyle = ComboBoxStyle.DropDownList;
            this.TablePanel.Controls.Add(AvailableShaders);

            var loadedShaders = ShaderLibraryCLI.GetIntance().GetAvailableShaderNames();
            
            foreach(String shaderName in loadedShaders) 
            {
                AvailableShaders.Items.Add(shaderName);
            }

            AvailableShaders.SelectedIndex = 0;


            Button okButton = new Button();
            okButton.Text = "Ok";
            this.TablePanel.Controls.Add(okButton);

            Button cancelButton = new Button();
            cancelButton.Text = "Cancel";
            this.TablePanel.Controls.Add(cancelButton);

            okButton.Click += new EventHandler(
                   delegate(object sender, EventArgs e)
                   {
                       if (CheckIfValid((String)AvailableShaders.Items[AvailableShaders.SelectedIndex], MaterialNameTextBox.Text)) 
                       {
                           this.Dispose();
                       }
                   }
               );

            cancelButton.Click += new EventHandler(
                   delegate(object sender, EventArgs e)
                   {
                       this.Dispose();
                   }
               );
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter)
            {
                if (CheckIfValid((String)AvailableShaders.Items[AvailableShaders.SelectedIndex], MaterialNameTextBox.Text))
                {
                    this.Dispose();
                }
                return true;
            }
            else if (keyData == Keys.Escape)
            {
                this.Dispose();
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private bool CheckIfValid(String selectedShader, String materialName) 
        {
            if (materialName.Length > 4)
            {
                Regex r = new Regex("^[a-zA-Z0-9_-]*$");
                if (r.IsMatch(materialName))
                {
                    String materialFileName = "Materials/" + materialName + ".json";
                    // Check if file exists.                   
                    if (FileSystemCLI.GetIntance().FileExists(materialFileName)) 
                    {
                        MessageBox.Show("Material name already exists.\nChoose another one!");
                    }
                    else
                    {
                        GenericSerializerCLI gs = new GenericSerializerCLI(materialFileName);
                        gs.SerializeEmptyMaterial(selectedShader, materialName);

                        this.MaterialListView.Items.Add(materialName);

                        Console.WriteLine("Material name {0} with Shader {1}", materialName, selectedShader);

                        return true;
                    }

                }
                else
                {
                    MessageBox.Show("Material name contains non alphanumeric characters.");
                }
            }
            else 
            {
                MessageBox.Show("Enter a material name with atleast 5 chars");
            }

            return false;
        }

        private void NewMaterialForm_Load(object sender, EventArgs e)
        {

        }
    }
}
