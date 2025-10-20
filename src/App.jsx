import { useState, useEffect } from "react";
import "./KKK.css";

function KKK() {
  const [data, setData] = useState([]);
  const [color, setColor] = useState("#000000");
  const [brightness, setBrightness] = useState(50);
  const [mode, setMode] = useState("normal"); // 🌈 New state for NeoPixel mode

  const serverUrl = "http://192.168.100.105:8000/List";

  const showDataJson = () => {
    fetch(serverUrl)
      .then((response) => {
        if (!response.ok) console.log("Error can't find server");
        return response.json();
      })
      .then((json) => {
        setData(json);
        if (json[0]) {
          setBrightness(Math.round((json[0].brightness * 100) / 255));
          setColor(json[0].color);
          setMode(json[0].display || "normal"); // Load mode from backend
        }
      });
  };

  const changeById = () => {
    const r = parseInt(color.substring(1, 3), 16);
    const g = parseInt(color.substring(3, 5), 16);
    const b = parseInt(color.substring(5, 7), 16);

    fetch(serverUrl, {
      method: "PATCH",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        id: 1,
        r,
        g,
        b,
        brightness: Math.round(Number(brightness) * 255 / 100),
        color,
        display: mode, // 👈 Send the selected mode to ESP32
      }),
    })
      .then((response) => {
        if (!response.ok) console.log("Error can't change using ID");
        return response.json();
      })
      .then((json) => setData(json));
  };

  useEffect(() => {
    showDataJson();
  }, []);

  return (
    <div className="container">
      <h1>LED Color Controller</h1>

      <div className="input-group">
        {/* Color Picker */}
        <label>Pick a Color:</label>
        <input
          type="color"
          value={color}
          onChange={(e) => setColor(e.target.value)}
          className="color-picker"
        />

        {/* Brightness Slider */}
        <div className="brightness-control">
          <label htmlFor="brightness">
            Brightness: <span>{brightness}%</span>
          </label>
          <input
            id="brightness" 
            type="range"
            min="0"
            max="100"
            value={brightness}
            onChange={(e) => setBrightness(e.target.value)}
            className="brightness-slider"
          />
        </div>


        {/* 🌈 NeoPixel Mode Selector */}
        <div className="mode-select">
          <label htmlFor="mode">Mode:</label>
          <select
            id="mode"
            value={mode}
            onChange={(e) => setMode(e.target.value)}
            className="mode-dropdown"
          >
            <option value="normal">Normal</option>
            <option value="blink">Blink</option>
            <option value="rainbow">Rainbow</option>
            <option value={"colorwipe"}>Color Wipe</option>
          </select>
        </div>


        <button onClick={changeById}>Apply</button>
      </div>

      {data[0] && (
        <>
          <h2>Current Color: {data[0].color}</h2>
          <p>
            RGB: ({data[0].r}, {data[0].g}, {data[0].b})
          </p>
          <p>Brightness: {Math.round((data[0].brightness * 100) / 255)}%</p>
          <p>Mode: {data[0].display}</p>
        </>
      )}
    </div>
  );
}

export default KKK;
