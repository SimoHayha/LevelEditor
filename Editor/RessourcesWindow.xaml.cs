using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Editor
{
	/// <summary>
	/// Logique d'interaction pour RessourcesWindow.xaml
	/// </summary>
	public partial class RessourcesWindow : Window
	{
		public ManagedLib.ManagedRessources Ressources { get; set; }

		public RessourcesWindow()
		{
			InitializeComponent();
			this.DataContext = this;
			Loaded += RessourcesWindow_Loaded;
		}

		void RessourcesWindow_Loaded(object sender, RoutedEventArgs e)
		{
			Ressources = new ManagedLib.ManagedRessources();
			listModels.ItemsSource = Ressources.FbxModels;
			listScript.ItemsSource = Ressources.Scripts;
			listTextures.ItemsSource = Ressources.Textures;
		}

		private void SaveScript(object sender, RoutedEventArgs e)
		{
			string path = listScript.SelectedItem as string;
			if (path == null)
				return;
			System.IO.File.WriteAllText(path, textScript.Text);
			Ressources.reloadScript(path);
		}

		private void listScript_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			string path = listScript.SelectedItem as string;
			if (path == null)
				return;
			textScript.Text = System.IO.File.ReadAllText(path);
		}

		private void NewScript(object sender, RoutedEventArgs e)
		{

		}
	}
}
