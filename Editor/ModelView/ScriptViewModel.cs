using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace Editor.ModelView
{
	public class ScriptViewModel : ToolViewModel
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

		public ScriptViewModel() :
			base("Script")
		{
			ContentId = ToolContentId;

			SelectionChangedCommand = new RelayCommand(this.SelectionChangedExecute);
			LoadButtonClickCommand = new RelayCommand(this.LoadButtonClickExecute);
			SaveButtonClickCommand = new RelayCommand(this.SaveButtonClickExecute);
		}

		public const string ToolContentId = "ScriptViewTool";

		public void OnLoad(ManagedLib.ManagedRessources ressources)
		{
			this.Ressources = ressources;
		}

		private ICommand selectionChangedCommand;
		public ICommand SelectionChangedCommand
		{ 
			get
			{
				return this.selectionChangedCommand;
			}
			set
			{
				this.selectionChangedCommand = value;
			}
		}

		public void SelectionChangedExecute(object obj)
		{
			string path = obj as string;
			if (path == null)
				return;
			Text = System.IO.File.ReadAllText(path);
		}

		private ICommand loadButtonClickCommand;
		public ICommand LoadButtonClickCommand
		{
			get
			{
				return this.loadButtonClickCommand;
			}
			set
			{
				this.loadButtonClickCommand = value;
			}
		}

		public void LoadButtonClickExecute(object obj)
		{
			OpenFileDialog dial = new OpenFileDialog();
			string path = null;

			dial.Filter = "Lua Script|*.lua";
			dial.Title = "Select a Script";

			if (System.Windows.Forms.DialogResult.OK == dial.ShowDialog())
				path = dial.FileName;
			Ressources.loadScript(path);
		}

		private ICommand saveButtonClickCommand;
		public ICommand SaveButtonClickCommand
		{
			get
			{
				return this.saveButtonClickCommand;
			}
			set
			{
				this.saveButtonClickCommand = value;
			}
		}

		public void SaveButtonClickExecute(object obj)
		{
			string path = obj as string;
			if (path == null)
				return;
			System.IO.File.WriteAllText(path, Text);
			Ressources.reloadScript(path);
		}
	}
}
