import React, { useState } from 'react';
import './NetworkScanning.css';

function NetworkScanning() {
  const [ipRange, setIpRange] = useState('');
  const [isScanning, setIsScanning] = useState(false);
  const [scanResults, setScanResults] = useState([]);

  const handleScan = async () => {
    setIsScanning(true);
    try {
      // Simulated API call - replace with actual scanning logic
      const response = await fetch('http://localhost:8080/scan', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ ipRange }),
      });
      
      const data = await response.json();
      setScanResults(data.results);
    } catch (error) {
      console.error('Scanning error:', error);
      // Handle error appropriately
    } finally {
      setIsScanning(false);
    }
  };

  const handleExport = () => {
    // Implement export functionality
    const exportData = JSON.stringify(scanResults, null, 2);
    const blob = new Blob([exportData], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'network-scan-results.json';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  return (
    <div className="network-scanning-container">
      <div className="scan-header">
        <h1 className="scan-title">Network Vulnerability Scanner</h1>
        <p className="scan-description">
          Scan your network for potential security vulnerabilities and open ports.
          Enter an IP range to begin assessment.
        </p>
      </div>

      <div className="scan-controls">
        <input
          type="text"
          className="scan-input"
          placeholder="Enter IP range (e.g., 192.168.1.1-192.168.1.254)"
          value={ipRange}
          onChange={(e) => setIpRange(e.target.value)}
        />
        <button
          className="scan-button"
          onClick={handleScan}
          disabled={isScanning || !ipRange}
        >
          {isScanning ? 'Scanning...' : 'Start Scan'}
        </button>
      </div>

      {scanResults.length > 0 && (
        <div className="results-section">
          <div className="results-header">
            <h2 className="results-title">Scan Results</h2>
            <button className="export-button" onClick={handleExport}>
              Export Results
            </button>
          </div>
          
          <div className="results-grid">
            {scanResults.map((vulnerability, index) => (
              <div key={index} className="vulnerability-card">
                <span className={`vulnerability-severity severity-${vulnerability.severity}`}>
                  {vulnerability.severity.toUpperCase()}
                </span>
                <h3 className="vulnerability-title">{vulnerability.title}</h3>
                <p className="vulnerability-description">{vulnerability.description}</p>
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}

export default NetworkScanning;