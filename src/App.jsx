import { useState } from "react";
import { huffmanCompress, huffmanDecompress } from "./huffman";
import "./App.css";

function App() {
  const [file, setFile] = useState(null);
  const [output, setOutput] = useState(null);
  const [mode, setMode] = useState("text");
  const [action, setAction] = useState("compress");
  const [sizeReduction, setSizeReduction] = useState(70);
  const [processing, setProcessing] = useState(false);

  const handleFileChange = (e) => {
    setFile(e.target.files[0]);
    setOutput(null);
  };

  const handleModeChange = (e) => {
    setMode(e.target.value);
    setFile(null);
    setOutput(null);
  };

  const handleActionChange = (e) => {
    setAction(e.target.value);
    setOutput(null);
  };

  const handleSizeReductionChange = (e) => {
    const value = Math.max(0, Math.min(100, parseInt(e.target.value) || 70));
    setSizeReduction(value);
  };

  const compressImageToTargetSize = async (file, targetPercentage) => {
    const img = new Image();
    const canvas = document.createElement("canvas");
    const ctx = canvas.getContext("2d");

    await new Promise((resolve) => {
      img.onload = () => {
        canvas.width = img.width;
        canvas.height = img.height;
        ctx.drawImage(img, 0, 0);
        resolve();
      };
      img.src = URL.createObjectURL(file);
    });

    const originalBlob = await fetch(URL.createObjectURL(file)).then((res) => res.blob());
    const originalSize = originalBlob.size;
    const targetSize = (originalSize * targetPercentage) / 100;

    if (targetPercentage === 100) {
      const url = canvas.toDataURL("image/jpeg", 1.0);
      return { url, name: `${file.name.split(".")[0]}_compressed.jpg` };
    }
    if (targetPercentage === 0) {
      return { url: "", name: null, note: "Target size of 0% is not possible" };
    }

    let low = 0.1;
    let high = 1.0;
    let url, blob, size;
    for (let i = 0; i < 10; i++) {
      const quality = (low + high) / 2;
      url = canvas.toDataURL("image/jpeg", quality);
      blob = await fetch(url).then((res) => res.blob());
      size = blob.size;

      if (size > targetSize) {
        high = quality;
      } else if (size < targetSize) {
        low = quality;
      } else {
        break;
      }
    }

    return { url, name: `${file.name.split(".")[0]}_compressed.jpg` };
  };

  const handleSubmit = async () => {
    if (!file) return;

    setProcessing(true);

    const arrayBuffer = await file.arrayBuffer();
    const inputData = new Uint8Array(arrayBuffer);

    if (mode === "text") {
      if (action === "compress") {
        const text = new TextDecoder().decode(inputData);
        const { encoded, codes } = huffmanCompress(text);
        const compressedData = JSON.stringify({ encoded, codes });
        const blob = new Blob([compressedData], { type: "application/json" });
        const url = URL.createObjectURL(blob);
        setOutput({ url, name: `${file.name.split(".")[0]}_compressed.json` });
      } else if (action === "decompress") {
        const compressedData = new TextDecoder().decode(inputData);
        const { encoded, codes } = JSON.parse(compressedData);
        const decoded = huffmanDecompress(encoded, codes);
        const blob = new Blob([decoded], { type: "text/plain" });
        const url = URL.createObjectURL(blob);
        setOutput({ url, name: "decompressed.txt" });
      }
    } else if (mode === "image") {
      const { url, name, note } = await compressImageToTargetSize(file, sizeReduction);
      setOutput({ url, name, note });
    }

    setProcessing(false);
  };

  return (
    <div className="container">
      <div className="header">
        <h1 className="title">PiedPiper</h1>
        <p className="tagline">Compress smarter, not harder</p>
      </div>
      <div className="upload-section">
        <div className="mode-choices">
          <label className="radio-label">
            <input
              type="radio"
              value="text"
              checked={mode === "text"}
              onChange={handleModeChange}
            />
            Text
          </label>
          <label className="radio-label">
            <input
              type="radio"
              value="image"
              checked={mode === "image"}
              onChange={handleModeChange}
            />
            Image
          </label>
        </div>
        <input
          type="file"
          accept={mode === "text" ? ".txt,.json" : ".png,.jpg,.jpeg"}
          onChange={handleFileChange}
          className="file-input"
          aria-label="Upload a file"
        />
        {mode === "text" ? (
          <div className="action-choices">
            <label className="radio-label">
              <input
                type="radio"
                value="compress"
                checked={action === "compress"}
                onChange={handleActionChange}
              />
              Compress
            </label>
            <label className="radio-label">
              <input
                type="radio"
                value="decompress"
                checked={action === "decompress"}
                onChange={handleActionChange}
              />
              Decompress
            </label>
          </div>
        ) : (
          <div className="size-reduction">
            <label htmlFor="sizeReduction">Target Size (%):</label>
            <input
              type="number"
              id="sizeReduction"
              value={sizeReduction}
              onChange={handleSizeReductionChange}
              min="0"
              max="100"
              className="size-input"
            />
          </div>
        )}
        <button
          onClick={handleSubmit}
          disabled={!file || processing}
          className="submit-btn"
        >
          {processing ? "Processing..." : "Process"}
        </button>
        {processing && <div className="progress-bar"></div>}
        {output && !processing && (
          <div className="output">
            {output.url && (
              <a href={output.url} download={output.name}>
                Download {output.name}
              </a>
            )}
            {output.note && <p className="note">{output.note}</p>}
            {mode === "image" && output.url && (
              <div className="preview">
                <img src={output.url} alt="Output preview" />
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  );
}

export default App;