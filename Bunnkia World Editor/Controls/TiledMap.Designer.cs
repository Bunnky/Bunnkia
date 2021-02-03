
namespace Bunnkia_World_Editor.Controls
{
    partial class TiledMap
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.widthUpDown = new System.Windows.Forms.NumericUpDown();
            this.heightUpDown = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.setsize_button = new System.Windows.Forms.Button();
            this.tileSizeUpDown = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tileStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tileInforStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.layerVisChkBox = new System.Windows.Forms.CheckBox();
            this.layerSelectBox = new System.Windows.Forms.ComboBox();
            this.zvalueUpDown = new System.Windows.Forms.NumericUpDown();
            this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.container_panel = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.widthUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tileSizeUpDown)).BeginInit();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.zvalueUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // widthUpDown
            // 
            this.widthUpDown.Location = new System.Drawing.Point(3, 20);
            this.widthUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.widthUpDown.Name = "widthUpDown";
            this.widthUpDown.Size = new System.Drawing.Size(75, 20);
            this.widthUpDown.TabIndex = 0;
            this.widthUpDown.ThousandsSeparator = true;
            this.widthUpDown.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // heightUpDown
            // 
            this.heightUpDown.Location = new System.Drawing.Point(87, 20);
            this.heightUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.heightUpDown.Name = "heightUpDown";
            this.heightUpDown.Size = new System.Drawing.Size(75, 20);
            this.heightUpDown.TabIndex = 1;
            this.heightUpDown.ThousandsSeparator = true;
            this.heightUpDown.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(3, 2);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 15);
            this.label1.TabIndex = 2;
            this.label1.Text = "Width";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(84, 2);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 15);
            this.label2.TabIndex = 3;
            this.label2.Text = "Height";
            // 
            // setsize_button
            // 
            this.setsize_button.Location = new System.Drawing.Point(168, 19);
            this.setsize_button.Name = "setsize_button";
            this.setsize_button.Size = new System.Drawing.Size(75, 22);
            this.setsize_button.TabIndex = 4;
            this.setsize_button.Text = "Set";
            this.setsize_button.UseVisualStyleBackColor = true;
            // 
            // tileSizeUpDown
            // 
            this.tileSizeUpDown.Location = new System.Drawing.Point(259, 20);
            this.tileSizeUpDown.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.tileSizeUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.tileSizeUpDown.Name = "tileSizeUpDown";
            this.tileSizeUpDown.Size = new System.Drawing.Size(56, 20);
            this.tileSizeUpDown.TabIndex = 5;
            this.tileSizeUpDown.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(256, 2);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 15);
            this.label3.TabIndex = 6;
            this.label3.Text = "Tilesize";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tileStatusLabel,
            this.tileInforStatusLabel});
            this.statusStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
            this.statusStrip1.Location = new System.Drawing.Point(0, 266);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.ManagerRenderMode;
            this.statusStrip1.Size = new System.Drawing.Size(331, 24);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 7;
            // 
            // tileStatusLabel
            // 
            this.tileStatusLabel.AutoSize = false;
            this.tileStatusLabel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Right;
            this.tileStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.tileStatusLabel.Name = "tileStatusLabel";
            this.tileStatusLabel.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
            this.tileStatusLabel.Size = new System.Drawing.Size(60, 17);
            this.tileStatusLabel.Text = "0,0";
            // 
            // tileInforStatusLabel
            // 
            this.tileInforStatusLabel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Right;
            this.tileInforStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.tileInforStatusLabel.Name = "tileInforStatusLabel";
            this.tileInforStatusLabel.Size = new System.Drawing.Size(40, 19);
            this.tileInforStatusLabel.Text = "None";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(3, 51);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(36, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Layer";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(165, 51);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Z-Value";
            // 
            // layerVisChkBox
            // 
            this.layerVisChkBox.AutoSize = true;
            this.layerVisChkBox.Location = new System.Drawing.Point(259, 67);
            this.layerVisChkBox.Name = "layerVisChkBox";
            this.layerVisChkBox.Size = new System.Drawing.Size(56, 17);
            this.layerVisChkBox.TabIndex = 10;
            this.layerVisChkBox.Text = "Visible";
            this.layerVisChkBox.UseVisualStyleBackColor = true;
            // 
            // layerSelectBox
            // 
            this.layerSelectBox.FormattingEnabled = true;
            this.layerSelectBox.Items.AddRange(new object[] {
            "Add.."});
            this.layerSelectBox.Location = new System.Drawing.Point(6, 67);
            this.layerSelectBox.Name = "layerSelectBox";
            this.layerSelectBox.Size = new System.Drawing.Size(156, 21);
            this.layerSelectBox.TabIndex = 12;
            // 
            // zvalueUpDown
            // 
            this.zvalueUpDown.DecimalPlaces = 2;
            this.zvalueUpDown.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.zvalueUpDown.Location = new System.Drawing.Point(168, 67);
            this.zvalueUpDown.Maximum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.zvalueUpDown.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            -2147483648});
            this.zvalueUpDown.Name = "zvalueUpDown";
            this.zvalueUpDown.Size = new System.Drawing.Size(75, 20);
            this.zvalueUpDown.TabIndex = 13;
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.hScrollBar1.LargeChange = 32;
            this.hScrollBar1.Location = new System.Drawing.Point(0, 249);
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(331, 17);
            this.hScrollBar1.SmallChange = 4;
            this.hScrollBar1.TabIndex = 14;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.LargeChange = 32;
            this.vScrollBar1.Location = new System.Drawing.Point(314, 96);
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(17, 153);
            this.vScrollBar1.SmallChange = 4;
            this.vScrollBar1.TabIndex = 15;
            // 
            // container_panel
            // 
            this.container_panel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.container_panel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.container_panel.Location = new System.Drawing.Point(0, 96);
            this.container_panel.Name = "container_panel";
            this.container_panel.Padding = new System.Windows.Forms.Padding(2);
            this.container_panel.Size = new System.Drawing.Size(314, 153);
            this.container_panel.TabIndex = 16;
            // 
            // TiledMap
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.container_panel);
            this.Controls.Add(this.vScrollBar1);
            this.Controls.Add(this.hScrollBar1);
            this.Controls.Add(this.zvalueUpDown);
            this.Controls.Add(this.layerSelectBox);
            this.Controls.Add(this.layerVisChkBox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tileSizeUpDown);
            this.Controls.Add(this.setsize_button);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.heightUpDown);
            this.Controls.Add(this.widthUpDown);
            this.DoubleBuffered = true;
            this.Name = "TiledMap";
            this.Padding = new System.Windows.Forms.Padding(0, 96, 0, 0);
            this.Size = new System.Drawing.Size(331, 290);
            ((System.ComponentModel.ISupportInitialize)(this.widthUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tileSizeUpDown)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.zvalueUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown widthUpDown;
        private System.Windows.Forms.NumericUpDown heightUpDown;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button setsize_button;
        private System.Windows.Forms.NumericUpDown tileSizeUpDown;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel tileStatusLabel;
        private System.Windows.Forms.ToolStripStatusLabel tileInforStatusLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox layerVisChkBox;
        private System.Windows.Forms.ComboBox layerSelectBox;
        private System.Windows.Forms.NumericUpDown zvalueUpDown;
        private System.Windows.Forms.HScrollBar hScrollBar1;
        private System.Windows.Forms.VScrollBar vScrollBar1;
        private System.Windows.Forms.Panel container_panel;
    }
}
