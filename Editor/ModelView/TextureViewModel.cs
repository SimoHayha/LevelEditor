using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace Editor.ModelView
{
	public class TextureViewModel : ToolViewModel
	{
		private string text;
		public string Text
		{
			get
			{
				return text;
			}
			set
			{
				text = value;
				RaisePropertyChanged("Text");
			}
		}

		public ManagedLib.ManagedRessources Ressources { get; set; }

		public TextureViewModel()
			: base("Textures")
		{
			ContentId = TextureContentId;
		}

		public void OnLoad(ManagedLib.ManagedRessources ressources)
		{
			this.Ressources = ressources;
			AddButtonClickCommand = new RelayCommand(this.AddButtonClickExecute);
		}

		public const string TextureContentId = "TextureTool";

		private ICommand addButtonClickCommand;
		public ICommand AddButtonClickCommand
		{
			get
			{
				return this.addButtonClickCommand;
			}
			set
			{
				this.addButtonClickCommand = value;
			}
		}

		public void AddButtonClickExecute(object obj)
		{
			OpenFileDialog dial = new OpenFileDialog();
			string path = null;

			dial.Filter = "All Files|*";
			dial.Title = "Select a Texture";

			if (System.Windows.Forms.DialogResult.OK == dial.ShowDialog())
				path = dial.FileName;
			Ressources.addTexture(path);
		}
	}
}
