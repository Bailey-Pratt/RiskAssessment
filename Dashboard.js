import React from 'react';
import { Link, Routes, Route } from 'react-router-dom';
import './Dashboard.css';
import NetworkScanning from './pages/NetworkScanning';
import PasswordAnalysis from './pages/PasswordAnalysis';
import SoftwareVulnerability from './pages/SoftwareVulnerability';
import FirewallConfig from './pages/FirewallConfig';
import Reports from './pages/Reports';
//import Settings from './pages/Settings';

// Dashboard Overview Component
function DashboardOverview() {
  return (
    <div className="overview-container">
      <section className="welcome-section">
        <h2 className="welcome-title">Security Assessment Overview</h2>
        <p className="welcome-text">
          Monitor your security metrics and assessment results across all modules.
        </p>
      </section>

      <div className="stats-grid">
        <div className="stat-card">
          <h3 className="stat-title">Total Assessments</h3>
          <p className="stat-value">1,234</p>
          <div className="stat-change positive">
            +12% from last month
          </div>
        </div>

        <div className="stat-card">
          <h3 className="stat-title">High Risk Cases</h3>
          <p className="stat-value">45</p>
          <div className="stat-change negative">
            +5% from last month
          </div>
        </div>

        <div className="stat-card">
          <h3 className="stat-title">Average Response Time</h3>
          <p className="stat-value">2.4h</p>
          <div className="stat-change positive">
            -15% from last month
          </div>
        </div>

        <div className="stat-card">
          <h3 className="stat-title">Completion Rate</h3>
          <p className="stat-value">94%</p>
          <div className="stat-change positive">
            +2% from last month
          </div>
        </div>
      </div>

      <div className="charts-grid">
        <div className="chart-container">
          <h3 className="chart-title">Risk Assessment Trends</h3>
          <div style={{ height: "300px", background: "#f8fafc", borderRadius: "0.375rem", display: "flex", alignItems: "center", justifyContent: "center" }}>
            Chart Placeholder
          </div>
        </div>

        <div className="chart-container">
          <h3 className="chart-title">Risk Distribution</h3>
          <div style={{ height: "300px", background: "#f8fafc", borderRadius: "0.375rem", display: "flex", alignItems: "center", justifyContent: "center" }}>
            Chart Placeholder
          </div>
        </div>
      </div>
    </div>
  );
}

// Main Dashboard Component
function Dashboard({ username, onLogout }) {
  return (
    <div className="dashboard">
      <header className="dashboard-header">
        <h1 className="header-title">Risk Assessment Dashboard</h1>
        <div className="header-actions">
          <span className="user-welcome">Welcome, {username}</span>
          <button onClick={onLogout} className="logout-button">
            Logout
          </button>
        </div>
      </header>

      <div className="dashboard-layout">
        <nav className="sidebar">
          <ul className="nav-links">
            <li>
              <Link to="/dashboard" className="nav-link">Overview</Link>
            </li>
            <li>
              <Link to="/dashboard/network" className="nav-link">Network Scanning</Link>
            </li>
            <li>
              <Link to="/dashboard/password" className="nav-link">Password Analysis</Link>
            </li>
            <li>
              <Link to="/dashboard/software" className="nav-link">Software Vulnerability</Link>
            </li>
            <li>
              <Link to="/dashboard/firewall" className="nav-link">Firewall Configuration</Link>
            </li>
            <li>
              <Link to="/dashboard/reports" className="nav-link">Reports</Link>
            </li>
            <li>
              <Link to="/dashboard/settings" className="nav-link">Settings</Link>
            </li>
          </ul>
        </nav>

        <main className="dashboard-content">
          <Routes>
            <Route path="/" element={<DashboardOverview />} />
            <Route path="network" element={<NetworkScanning />} />
            <Route path="password" element={<PasswordAnalysis />} />
            <Route path="software" element={<SoftwareVulnerability />} />
            <Route path="firewall" element={<FirewallConfig />} />
            <Route path="reports" element={<Reports />} />
            
          </Routes>
        </main>
      </div>
    </div>
  );
}

export default Dashboard;