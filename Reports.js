import React, { useState, useEffect } from 'react';
import './Reports.css';

function Reports() {
  const [reports, setReports] = useState([]);
  const [isLoading, setIsLoading] = useState(true);
  const [selectedReport, setSelectedReport] = useState(null);
  const [dateRange, setDateRange] = useState('last7days');
  const [reportType, setReportType] = useState('all');

  useEffect(() => {
    fetchReports();
  }, [dateRange, reportType]);

  const fetchReports = async () => {
    setIsLoading(true);
    try {
      const response = await fetch(`http://localhost:8080/reports?range=${dateRange}&type=${reportType}`);
      const data = await response.json();
      setReports(data.reports || []);
    } catch (error) {
      console.error('Error fetching reports:', error);
    } finally {
      setIsLoading(false);
    }
  };

  const generateReport = async () => {
    try {
      const response = await fetch('http://localhost:8080/generate-report', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ type: reportType, dateRange }),
      });
      
      if (response.ok) {
        fetchReports();
      }
    } catch (error) {
      console.error('Error generating report:', error);
    }
  };

  return (
    <div className="reports-container">
      <div className="reports-header">
        <h1 className="reports-title">Security Assessment Reports</h1>
        <p className="reports-description">
          View and generate comprehensive security assessment reports and analytics.
        </p>
      </div>

      <div className="controls-section">
        <div className="filters">
          <select
            value={dateRange}
            onChange={(e) => setDateRange(e.target.value)}
            className="filter-select"
          >
            <option value="last7days">Last 7 Days</option>
            <option value="last30days">Last 30 Days</option>
            <option value="last90days">Last 90 Days</option>
            <option value="custom">Custom Range</option>
          </select>

          <select
            value={reportType}
            onChange={(e) => setReportType(e.target.value)}
            className="filter-select"
          >
            <option value="all">All Reports</option>
            <option value="network">Network Security</option>
            <option value="software">Software Vulnerability</option>
            <option value="firewall">Firewall Analysis</option>
            <option value="password">Password Assessment</option>
          </select>

          <button onClick={generateReport} className="generate-button">
            Generate New Report
          </button>
        </div>
      </div>

      <div className="reports-content">
        {isLoading ? (
          <div className="loading">Loading reports...</div>
        ) : (
          <div className="reports-grid">
            {reports.map((report, index) => (
              <div 
                key={index} 
                className="report-card"
                onClick={() => setSelectedReport(report)}
              >
                <div className="report-icon">📊</div>
                <div className="report-info">
                  <h3 className="report-name">{report.name}</h3>
                  <p className="report-date">{new Date(report.date).toLocaleDateString()}</p>
                  <p className="report-type">{report.type}</p>
                </div>
                <div className="report-actions">
                  <button className="action-button view">View</button>
                  <button className="action-button download">Download</button>
                </div>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

export default Reports;