using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using SupraHot.CLI;

namespace SupraHotEditor
{
    class MaterialPropertyWidget : Panel
    {
        private MaterialPropertyCommonInterface MaterialPropertyCommonInterface = null;
        private String Type = "";
        private String Name = "";
        private FlowLayoutPanel groupBoxFlowLayout;
        private MaterialView MaterialView;

        public MaterialPropertyWidget(MaterialPropertyCommonInterface materialPropertyCommonInterface, MaterialView parentMaterialView)
        {
            this.MaterialPropertyCommonInterface = materialPropertyCommonInterface;
            this.MaterialView = parentMaterialView;

            this.Type = materialPropertyCommonInterface.GetType();
            this.Name = materialPropertyCommonInterface.GetName();
            this.Text = materialPropertyCommonInterface.GetName() + " (" + this.Type + ")";
            this.Width = 130;
            this.MinimumSize = new Size(this.Width, 150);

            groupBoxFlowLayout = new FlowLayoutPanel();
            groupBoxFlowLayout.Dock = DockStyle.Fill;
            groupBoxFlowLayout.FlowDirection = FlowDirection.TopDown;
            groupBoxFlowLayout.WrapContents = false;
            this.Controls.Add(groupBoxFlowLayout);

            Color lightGrey = Color.FromArgb(20, 20, 20);
            Color whiteColor = Color.FromArgb(255, 255, 255);
            this.BackColor = lightGrey;
            this.ForeColor = whiteColor;

            // Remove Button
            Button removeButton = new Button();
            removeButton.Text = "X";
            removeButton.Width = 20;
            removeButton.Height = 20;
            removeButton.FlatStyle = FlatStyle.Flat;
            removeButton.FlatAppearance.BorderSize = 1;
            removeButton.Dock = DockStyle.Right;
            removeButton.Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Console.WriteLine("Clicked remove {0} button!", this.Text);

                        // Remove MaterialProperty
                        // Also, add this MaterialProperty back to it's original List, so that we can add it later, if we want to.

                        this.MaterialView.RemoveMaterialProperty(Name, MaterialPropertyCommonInterface, this);

                        Form1.UpdateView();
                    }
                );
            groupBoxFlowLayout.Controls.Add(removeButton);

            // Name
            Label nameLabel = new Label();
            nameLabel.Text = this.Text;
            groupBoxFlowLayout.Controls.Add(nameLabel);



            if (Type == "Boolean")
            {
                RadioButton trueButton = new RadioButton();
                trueButton.Text = "True";
                trueButton.Margin = new Padding(0, 0, 0, 0);

                RadioButton falseButton = new RadioButton();
                falseButton.Text = "False";
                falseButton.Margin = new Padding(0, 0, 0, 0);
                falseButton.Checked = true;

                groupBoxFlowLayout.Controls.Add(trueButton);
                groupBoxFlowLayout.Controls.Add(falseButton);

                if (((BooleanMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue())
                {
                    trueButton.Checked = true;
                }
                else
                {
                    falseButton.Checked = true;
                }

                trueButton.CheckedChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {

                        if (trueButton.Checked)
                        {
                            ((BooleanMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(true);
                        }
                        else if (falseButton.Checked)
                        {
                            ((BooleanMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(false);
                        }

                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Float")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                xSpinner.Value = (decimal)((FloatMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        ((FloatMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue((float)xSpinner.Value);
                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Vec2")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                this.Height = 200;

                Vec2CLI val = ((Vec2MaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec2CLI result = new Vec2CLI((float)xSpinner.Value, (float)ySpinner.Value);
                        ((Vec2MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec2CLI result = new Vec2CLI((float)xSpinner.Value, (float)ySpinner.Value);
                        ((Vec2MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

            }
            else if (Type == "Vec3")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                groupBoxFlowLayout.Controls.Add(zLabel);
                groupBoxFlowLayout.Controls.Add(zSpinner);

                this.Height = 200;

                Vec3CLI val = ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Vec4")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                Label wLabel = new Label();
                wLabel.Text = "W";

                NumericUpDown wSpinner = new NumericUpDown();
                wSpinner.DecimalPlaces = 6;
                wSpinner.Increment = 0.1M;
                wSpinner.Maximum = (decimal)10000000000000.0f;
                wSpinner.Minimum = -wSpinner.Maximum;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                groupBoxFlowLayout.Controls.Add(zLabel);
                groupBoxFlowLayout.Controls.Add(zSpinner);

                groupBoxFlowLayout.Controls.Add(wLabel);
                groupBoxFlowLayout.Controls.Add(wSpinner);

                this.Height = 250;

                Vec4CLI val = ((Vec4MaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;
                wSpinner.Value = (decimal)val.w;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec4CLI result = new Vec4CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value, (float)wSpinner.Value);
                        ((Vec4MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec4CLI result = new Vec4CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value, (float)wSpinner.Value);
                        ((Vec4MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec4CLI result = new Vec4CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value, (float)wSpinner.Value);
                        ((Vec4MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                wSpinner.ValueChanged += new EventHandler(
                   delegate(object sender, EventArgs e)
                   {
                       Vec4CLI result = new Vec4CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value, (float)wSpinner.Value);
                       ((Vec4MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                       Form1.UpdateView();
                   }
               );
            }
            else if (Type == "Texture2D")
            {
                Label pathLabel = new Label();
                pathLabel.Text = "Name: Default";

                if (((Texture2DMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue() != "")
                {
                    pathLabel.Text = ((Texture2DMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();
                }

                Button loadImageButton = new Button();
                loadImageButton.Text = "Load";

                OpenFileDialog openFileDialog = new OpenFileDialog();

                loadImageButton.Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        openFileDialog.Filter = "Image|*.dds; *.png";

                        if (openFileDialog.ShowDialog() == DialogResult.OK)
                        {
                            String fileName = openFileDialog.SafeFileName;
                            String filePath = openFileDialog.FileName;

                            // Check if the selected file exists in our content folder.
                            // If it doesn't we need to copy it over.

                            FileSystemCLI fileSystem = FileSystemCLI.GetIntance();
                            String fileSystemRootPath = fileSystem.GetRootPath();

                            filePath = filePath.Replace("\\", "/");

                            if (filePath.Contains("Content/"))
                            {
                                var splittedString = filePath.Split(new[] { "Content/" }, StringSplitOptions.None);

                                if (splittedString.Length > 1)
                                {
                                    var pathToFileInRootSys = splittedString[1];

                                    bool isInRootSystem = fileSystem.FileExists(pathToFileInRootSys);

                                    if (isInRootSystem)
                                    {
                                        ((Texture2DMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(pathToFileInRootSys);
                                        pathLabel.Text = pathToFileInRootSys;
                                    }
                                    else
                                    {
                                        Console.WriteLine("{0} is not in the Content/ directory. Please move it there", fileName);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("{0} is not in the Content/ directory. Please move it there", fileName);
                            }

                        }

                        Form1.UpdateView();
                    }
                );

                groupBoxFlowLayout.Controls.Add(pathLabel);

                groupBoxFlowLayout.Controls.Add(loadImageButton);
            }
            else if (Type == "TextureCube")
            {
                Label pathLabel = new Label();
                pathLabel.Text = "Name: Default";


                if (((TextureCubeMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue() != "")
                {
                    pathLabel.Text = ((TextureCubeMaterialPropertyCLI)this.MaterialPropertyCommonInterface).GetValue();
                }

                Button loadImageButton = new Button();
                loadImageButton.Text = "Load";

                OpenFileDialog openFileDialog = new OpenFileDialog();

                loadImageButton.Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        openFileDialog.Filter = "Image|*.dds";

                        if (openFileDialog.ShowDialog() == DialogResult.OK)
                        {
                            String fileName = openFileDialog.SafeFileName;
                            String filePath = openFileDialog.FileName;

                            // Check if the selected file exists in our content folder.
                            // If it doesn't we need to copy it over.

                            FileSystemCLI fileSystem = FileSystemCLI.GetIntance();
                            String fileSystemRootPath = fileSystem.GetRootPath();

                            filePath = filePath.Replace("\\", "/");

                            if (filePath.Contains("Content/"))
                            {
                                var splittedString = filePath.Split(new[] { "Content/" }, StringSplitOptions.None);

                                if (splittedString.Length > 1)
                                {
                                    var pathToFileInRootSys = splittedString[1];

                                    bool isInRootSystem = fileSystem.FileExists(pathToFileInRootSys);

                                    if (isInRootSystem)
                                    {
                                        ((TextureCubeMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(pathToFileInRootSys);
                                        pathLabel.Text = pathToFileInRootSys;
                                    }
                                    else
                                    {
                                        Console.WriteLine("{0} is not in the Content/ directory. Please move it there", fileName);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("{0} is not in the Content/ directory. Please move it there", fileName);
                            }

                        }

                        Form1.UpdateView();
                    }
                );

                groupBoxFlowLayout.Controls.Add(pathLabel);

                groupBoxFlowLayout.Controls.Add(loadImageButton);
            }
        }

        public String GetType() 
        {
            return Type;
        }

        public String GetName() 
        {
            return Name;
        }

        public MaterialPropertyCommonInterface GetMaterialPropertyCommonInterface() 
        {
            return MaterialPropertyCommonInterface;
        }
    }
}
