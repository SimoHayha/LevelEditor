using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    [Serializable()]
    public class SerializableEntity
    {
        [Serializable()]
        public struct float3
        {
            public float x { get; set; }
            public float y { get; set; }
            public float z { get; set; }
        }
        public ObservableCollection<SerializableEntity> Childs { get; set; }
        public float3 Translation { get; set; }
        public float3 Rotation { get; set; }
        public float3 Scale { get; set; }
        public float3 Color { get; set; }
        public string Path { get; set; }
        public int Type { get; set; }

        public SerializableEntity(ManagedLib.ManagedEntity managed)
        {
            float3 temp;
            temp = new float3();
            temp.x = managed.TransX;
            temp.y = managed.TransY;
            temp.z = managed.TransZ;
            Translation = temp;
            temp = new float3();
            temp.x = managed.RotX;
            temp.y = managed.RotY;
            temp.z = managed.RotZ;
            Rotation = temp;
            temp = new float3();
            temp.x = managed.ScaleX;
            temp.y = managed.ScaleY;
            temp.z = managed.ScaleZ;
            Scale = temp;
            Type = managed.Type;
            Path = managed.Path;
            Childs = new ObservableCollection<SerializableEntity>();
        }

        public void FillEntity(ManagedLib.ManagedEntity managed)
        {
            managed.TransX = Translation.x;
            managed.TransY = Translation.y;
            managed.TransZ = Translation.z;
            managed.RotX = Rotation.x;
            managed.RotY = Rotation.y;
            managed.RotZ = Rotation.z;
            managed.ScaleX = Scale.x;
            managed.ScaleY = Scale.y;
            managed.ScaleZ = Scale.z;
        }

        public SerializableEntity()
        {
        }
    }
}
