using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace Editor
{
	class RadianToDegreeConvertor : IValueConverter
	{
		float degree;
		float radian;
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			radian = (float)value;
			degree = (radian * 180) / (float)Math.PI;
			degree = ((degree % 360) + 360) % 360;
			return degree;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
            string toConvert;
			radian = 0;

            toConvert = value as string;
            if (toConvert == null)
            {
                toConvert = System.Convert.ToString(value);
            }

			if (float.TryParse(toConvert, System.Globalization.NumberStyles.AllowThousands | System.Globalization.NumberStyles.Float, null, out degree))
				radian = (degree * (float)Math.PI) / 180;
			radian = ((radian % (2 * (float)Math.PI)) + (2 * (float)Math.PI)) % (2 * (float)Math.PI);
			return radian;
		}
	}
}
