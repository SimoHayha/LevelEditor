using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Xceed.Wpf.AvalonDock.Layout;

namespace Editor.ModelView
{
    public class PanesTemplateSelector : DataTemplateSelector
    {
        public PanesTemplateSelector()
        {
        }

        public DataTemplate TreeViewTemplate
        {
            get;
            set;
        }

        public DataTemplate PropertyViewTemplate
        {
            get;
            set;
        }

        public DataTemplate DXViewTemplate
        {
            get;
            set;
        }

        public DataTemplate AnimationViewTemplate
        {
            get;
            set;
        }

        public DataTemplate ConsoleViewTemplate
        {
            get;
            set;
        }
		public DataTemplate MaterialViewTemplate
		{
			get;
			set;
		}

		public DataTemplate TextureViewTemplate
		{
			get;
			set;
		}

        public DataTemplate PerfViewTemplate
        {
            get;
            set;
        }

		public DataTemplate ScriptViewTemplate
		{
			get;
			set;
		}

		public DataTemplate ViewPortViewTemplate
		{
			get;
			set;
		}

		public DataTemplate ShaderViewTemplate
		{
			get;
			set;
		}

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            var itemAsLayoutContent = item as LayoutContent;

            if (item is TreeViewModel)
                return TreeViewTemplate;

            if (item is PropertyViewModel)
                return PropertyViewTemplate;

            if (item is AnimationViewModel)
                return AnimationViewTemplate;

            if (item is ConsoleViewModel)
                return ConsoleViewTemplate;

			if (item is MaterialViewModel)
				return MaterialViewTemplate;

			if (item is TextureViewModel)
				return TextureViewTemplate;

            if (item is PerfViewModel)
                return PerfViewTemplate;

			if (item is ScriptViewModel)
				return ScriptViewTemplate;

			if (item is ViewPortViewModel)
				return ViewPortViewTemplate;

			if (item is ShaderViewModel)
				return ShaderViewTemplate;

            return base.SelectTemplate(item, container);
        }
    }
}
