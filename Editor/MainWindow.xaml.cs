
using Editor.ModelView;
using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Xml.Serialization;
using WpfHostingWin32Control;

namespace Editor
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        private WpfHostingWin32Control.ControlHost control = null;

        public MainWindow()
        {
            InitializeComponent();

			Workspace ws = Workspace.This;
            this.DataContext = ws;
			if (ws.MenuExitAction == null)
				ws.MenuExitAction = new Action(() => this.Close());

            this.Loaded += MainWindow_Loaded;
            this.Unloaded += MainWindow_Unloaded;

            CompositionTarget.Rendering += CompositionTarget_Rendering;
        }

        void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            if (control != null)
                control.render();
        }

        void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            bool error = false;

            control = new ControlHost(ControlHostElement.ActualHeight, ControlHostElement.ActualWidth, this);
            try
            {
                ControlHostElement.Child = control;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(ex.Message);
                this.control = null;
                error = true;
                //System.Windows.Application.Current.Shutdown(-1);
                //return;
            }

            Workspace workspace = this.DataContext as Workspace;

            if (null != workspace)
                workspace.OnLoad(this.control);

            if (!error)
            {
                var serializer = new Xceed.Wpf.AvalonDock.Layout.Serialization.XmlLayoutSerializer(dockManager);
                serializer.LayoutSerializationCallback += (s, args) =>
                    {
                        if (args.Model.ContentId == "PropertyViewTool")
                            args.Content = args.Content;
                        args.Content = args.Content;
                    };

                if (File.Exists(@".\user.cfg"))
                    serializer.Deserialize(@".\user.cfg");
            }
        }

        void MainWindow_Unloaded(object sender, RoutedEventArgs e)
        {
            var serializer = new Xceed.Wpf.AvalonDock.Layout.Serialization.XmlLayoutSerializer(dockManager);
            serializer.Serialize(@".\user.cfg");
        }

        private void MenuItem_Click_Show(object sender, RoutedEventArgs e)
        {
            var newWindow = new RessourcesWindow();
            newWindow.Show();
        }

        private void ComboBox_Selected(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.ComboBox box = sender as System.Windows.Controls.ComboBox;
            if (box == null)
                return;
        }

        private void AddViewport(object sender, RoutedEventArgs e)
        {
            control.ViewportChange(1);
        }

        private void RemoveViewport(object sender, RoutedEventArgs e)
        {
            control.ViewportChange(-1);
        }

        private void ControlHostElement_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (control == null)
                return;
            control.Resize((int)ControlHostElement.ActualHeight, (int)ControlHostElement.ActualWidth);
        }
    }
}
