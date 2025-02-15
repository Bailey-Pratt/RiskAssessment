import React, { useState, useEffect } from 'react';
import './FirewallConfig.css';

function FirewallConfig() {
  const [rules, setRules] = useState([]);
  const [newRule, setNewRule] = useState({
    name: '',
    sourceIp: '',
    destinationIp: '',
    port: '',
    protocol: 'TCP',
    action: 'ALLOW'
  });
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    fetchRules();
  }, []);

  const fetchRules = async () => {
    try {
      const response = await fetch('http://localhost:8080/firewall-rules');
      const data = await response.json();
      setRules(data.rules || []);
    } catch (error) {
      console.error('Error fetching rules:', error);
    } finally {
      setIsLoading(false);
    }
  };

  const handleAddRule = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('http://localhost:8080/firewall-rules', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(newRule),
      });
      
      if (response.ok) {
        fetchRules();
        setNewRule({
          name: '',
          sourceIp: '',
          destinationIp: '',
          port: '',
          protocol: 'TCP',
          action: 'ALLOW'
        });
      }
    } catch (error) {
      console.error('Error adding rule:', error);
    }
  };

  return (
    <div className="firewall-config-container">
      <div className="config-header">
        <h1 className="config-title">Firewall Configuration</h1>
        <p className="config-description">
          Manage your firewall rules and security policies to control network traffic.
        </p>
      </div>

      <div className="firewall-content">
        <div className="rules-section">
          <div className="section-header">
            <h2 className="section-title">Active Rules</h2>
            <button className="refresh-button" onClick={fetchRules}>
              Refresh Rules
            </button>
          </div>

          {isLoading ? (
            <div className="loading">Loading rules...</div>
          ) : (
            <div className="rules-table">
              <div className="table-header">
                <div className="header-cell">Name</div>
                <div className="header-cell">Source IP</div>
                <div className="header-cell">Destination IP</div>
                <div className="header-cell">Port</div>
                <div className="header-cell">Protocol</div>
                <div className="header-cell">Action</div>
                <div className="header-cell">Controls</div>
              </div>
              
              {rules.map((rule, index) => (
                <div key={index} className="table-row">
                  <div className="cell">{rule.name}</div>
                  <div className="cell">{rule.sourceIp}</div>
                  <div className="cell">{rule.destinationIp}</div>
                  <div className="cell">{rule.port}</div>
                  <div className="cell">{rule.protocol}</div>
                  <div className="cell">
                    <span className={`action-badge ${rule.action.toLowerCase()}`}>
                      {rule.action}
                    </span>
                  </div>
                  <div className="cell">
                    <button className="rule-button edit">Edit</button>
                    <button className="rule-button delete">Delete</button>
                  </div>
                </div>
              ))}
            </div>
          )}
        </div>

        <div className="add-rule-section">
          <h2 className="section-title">Add New Rule</h2>
          <form onSubmit={handleAddRule} className="rule-form">
            <div className="form-row">
              <div className="form-group">
                <label>Rule Name</label>
                <input
                  type="text"
                  value={newRule.name}
                  onChange={(e) => setNewRule({...newRule, name: e.target.value})}
                  placeholder="Enter rule name"
                  required
                />
              </div>
              
              <div className="form-group">
                <label>Source IP</label>
                <input
                  type="text"
                  value={newRule.sourceIp}
                  onChange={(e) => setNewRule({...newRule, sourceIp: e.target.value})}
                  placeholder="e.g., 192.168.1.0/24"
                  required
                />
              </div>
            </div>

            <div className="form-row">
              <div className="form-group">
                <label>Destination IP</label>
                <input
                  type="text"
                  value={newRule.destinationIp}
                  onChange={(e) => setNewRule({...newRule, destinationIp: e.target.value})}
                  placeholder="e.g., 10.0.0.0/8"
                  required
                />
              </div>
              
              <div className="form-group">
                <label>Port</label>
                <input
                  type="text"
                  value={newRule.port}
                  onChange={(e) => setNewRule({...newRule, port: e.target.value})}
                  placeholder="e.g., 80,443 or 1-1024"
                  required
                />
              </div>
            </div>

            <div className="form-row">
              <div className="form-group">
                <label>Protocol</label>
                <select
                  value={newRule.protocol}
                  onChange={(e) => setNewRule({...newRule, protocol: e.target.value})}
                >
                  <option value="TCP">TCP</option>
                  <option value="UDP">UDP</option>
                  <option value="ICMP">ICMP</option>
                  <option value="ALL">ALL</option>
                </select>
              </div>
              
              <div className="form-group">
                <label>Action</label>
                <select
                  value={newRule.action}
                  onChange={(e) => setNewRule({...newRule, action: e.target.value})}
                >
                  <option value="ALLOW">ALLOW</option>
                  <option value="DENY">DENY</option>
                  <option value="REJECT">REJECT</option>
                </select>
              </div>
            </div>

            <button type="submit" className="submit-button">
              Add Rule
            </button>
          </form>
        </div>
      </div>
    </div>
  );
}

export default FirewallConfig;