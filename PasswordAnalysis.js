import React, { useState } from 'react';
import './PasswordAnalysis.css';

function PasswordAnalysis() {
  const [password, setPassword] = useState('');
  const [analysisResults, setAnalysisResults] = useState(null);
  const [isAnalyzing, setIsAnalyzing] = useState(false);

  const analyzePassword = async () => {
    setIsAnalyzing(true);
    try {
      const response = await fetch('http://localhost:8080/analyze-password', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ password }),
      });
      
      const data = await response.json();
      setAnalysisResults(data);
    } catch (error) {
      console.error('Analysis error:', error);
    } finally {
      setIsAnalyzing(false);
    }
  };

  return (
    <div className="password-analysis-container">
      <div className="analysis-header">
        <h1 className="analysis-title">Password Security Analysis</h1>
        <p className="analysis-description">
          Analyze password strength and security compliance.
        </p>
      </div>

      <div className="analysis-form">
        <div className="input-group">
          <label className="input-label">Password to Analyze</label>
          <input
            type="password"
            className="password-input"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            placeholder="Enter password to analyze"
          />
        </div>
        <button
          className="analyze-button"
          onClick={analyzePassword}
          disabled={isAnalyzing || !password}
        >
          {isAnalyzing ? 'Analyzing...' : 'Analyze Password'}
        </button>
      </div>

      {analysisResults && (
        <div className="results-section">
          <div className="strength-meter">
            <div className="strength-label">
              <span className="strength-text">Password Strength</span>
              <span className="strength-score">{analysisResults.score}/100</span>
            </div>
            <div className="strength-bar">
              <div
                className={`strength-fill strength-${analysisResults.rating}`}
                style={{ width: `${analysisResults.score}%` }}
              />
            </div>
          </div>

          <ul className="analysis-list">
            {analysisResults.checks.map((check, index) => (
              <li key={index} className="analysis-item">
                <span className={`item-icon ${check.passed ? 'passed' : 'failed'}`}>
                  {check.passed ? '✓' : '✗'}
                </span>
                <span className="item-text">{check.description}</span>
              </li>
            ))}
          </ul>

          {analysisResults.recommendations && (
            <div className="recommendations">
              <h3 className="recommendations-title">Recommendations</h3>
              {analysisResults.recommendations.map((rec, index) => (
                <p key={index} className="recommendation-item">
                  {rec}
                </p>
              ))}
            </div>
          )}
        </div>
      )}
    </div>
  );
}

export default PasswordAnalysis;