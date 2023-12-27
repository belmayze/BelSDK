using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace ShaderConverter
{
    public class XmlData
    {
        /// <summary>
        /// シェーダー固有の設定
        /// </summary>
        /// <summary>
        /// シェーダープログラムの設定
        /// </summary>
        public class Program
        {
            public class Shader
            {
                [XmlAttribute("Source")]
                public string SourceFilePath { get; set; } = "";
            }

            [XmlAttribute("Name")]
            public string Name { get; set; } = "";
            [XmlElement("Vertex")]
            public Shader? VertexShader { get; set; } = null;
            [XmlElement("Geometry")]
            public Shader? GeometryShader { get; set; } = null;
            [XmlElement("Pixel")]
            public Shader? PixelShader { get; set; } = null;
            [XmlElement("Compute")]
            public Shader? ComputeShader { get; set; } = null;
            [XmlElement("Domain")]
            public Shader? DomainShader { get; set; } = null;
            [XmlElement("Hull")]
            public Shader? HullShader { get; set; } = null;
            [XmlElement("Amplification")]
            public Shader? AmplificationShader { get; set; } = null;
            [XmlElement("Mesh")]
            public Shader? MeshShader { get; set; } = null;
        }

        /// <summary>
        /// ファイルの読み込みパス
        /// </summary>
        public class Include
        {
            [XmlAttribute("Path")]
            public string Path { get; set; } = "";
        }

        /// <summary>
        /// コンパイル設定
        /// </summary>
        public class Compile
        {
            [XmlAttribute("Profile")]
            public string Profile { get; set; } = "5_0";
        }

        /// <summary>
        /// シェーダーの設定
        /// </summary>
        public class Settings
        {
            [XmlArray("IncludeArray")]
            [XmlArrayItem("Include")]
            public List<Include> Includes = new List<Include>();

            [XmlElement("Compile")]
            public Compile Compile = new Compile();
        }

        /// <summary>
        /// シェーダー全体の設定
        /// </summary>
        [XmlRoot("Shader")]
        public class ShaderContainer
        {
            [XmlElement("Settings")]
            public Settings Settings = new Settings();

            [XmlArray("ProgramArray")]
            [XmlArrayItem("Program")]
            public List<Program> Programs = new List<Program>();
        }
    }
}
