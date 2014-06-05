using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ModelView
{
	public class ShaderViewModel : ToolViewModel
	{
		private string textVs;
		public string TextVs
		{
			get
			{
				return textVs;
			}
			set
			{
				textVs = value;
				RaisePropertyChanged("TextVs");
			}
		}

		private string textPs;
		public string TextPs
		{
			get
			{
				return textPs;
			}
			set
			{
				textPs = value;
				RaisePropertyChanged("TextPs");
			}
		}

		public ManagedLib.ManagedRessources Ressources { get; set; }

		public ShaderViewModel() :
			base("Shader")
		{
			ContentId = ToolContentId;

			ReloadButtonClickCommand = new RelayCommand(ReloadButtonClickExecute);
		}

		public const string ToolContentId = "ShaderViewTool";

		internal void OnLoad()
		{
			
		}

		private ICommand reloadButtonClickCommand;
		public ICommand ReloadButtonClickCommand
		{
			get
			{
				return this.reloadButtonClickCommand;
			}
			set
			{
				this.reloadButtonClickCommand = value;
			}
		}

		public void ReloadButtonClickExecute(object obj)
		{
			
		}
	}
}
