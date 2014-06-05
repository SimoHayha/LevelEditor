using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ModelView
{
	public class MaterialViewModel : ToolViewModel
	{

		public ManagedLib.ManagedRessources Ressources { get; set; }

		public MaterialViewModel()
			: base("Materials")
		{
			ContentId = ToolContentId;
		}

		private ManagedLib.ManagedEntity selectedEntity = null;
		public ManagedLib.ManagedEntity SelectedEntity
		{
			get
			{
				return this.selectedEntity;
			}
			set
			{
				this.selectedEntity = value;
				RaisePropertyChanged("SelectedEntity");
			}
		}

		public void OnLoad(ManagedLib.ManagedRessources ressources)
		{
			this.Ressources = ressources;
		}

		public const string ToolContentId = "MaterialTool";
	}
}
