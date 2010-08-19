﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.DirectoryServices;
using System.IO;

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for ProdWnd.xaml
    /// </summary>
    public partial class ProdWnd : Window
    {
        public ProdWnd()
        {
            InitializeComponent();
            imglist = new Dictionary<string, string>();
            
            foreach (string f in Directory.GetFiles("f:\\","*.jpg"))
            {
                FileInfo fi = new FileInfo(f);
                string nm = fi.Name.Remove(fi.Name.Length-4,4);
                imglist[nm] = fi.FullName;
                Button n = new Button();
                ControlTemplate ct = this.FindResource("imgBtn") as ControlTemplate;
                n.Template = ct;
                n.Name = "ID"+nm;
                n.Content = nm;
                n.Click += ImageSelected;
                n.ApplyTemplate();
                ImageBrush ib = ct.FindName("ImageFill",n) as ImageBrush;
                ib.ImageSource = new BitmapImage(new Uri(fi.FullName));
                this.wrapPanel1.Children.Add(n);
            }
            
        }
        private Dictionary<string, string> imglist;
        private void ImageSelected(object sender, EventArgs arg)
        {
            MessageBox.Show((sender as Button).Name);
        }
    }
}
