import React, { useState } from 'react';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import Dashboard from './Dashboard';
import './App.css';

function App() {
  const [isLogin, setIsLogin] = useState(true);
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [message, setMessage] = useState('');
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [userId, setUserId] = useState(null);
  const [isLoading, setIsLoading] = useState(false);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setIsLoading(true);
    const endpoint = isLogin ? '/login' : '/register';
    
    try {
      const response = await fetch(`http://localhost:8080${endpoint}`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: `username=${encodeURIComponent(username)}&password=${encodeURIComponent(password)}`
      });
      
      const data = await response.json();
      
      if (data.status === 'success') {
        setMessage(data.message);
        if (isLogin) {
          setIsLoggedIn(true);
          setUserId(data.userId);
        } else {
          setIsLogin(true);
        }
      } else {
        setMessage(data.message);
      }
    } catch (error) {
      setMessage('Error connecting to server: ' + error.message);
    } finally {
      setIsLoading(false);
    }
  };

  if (isLoggedIn) {
    return (
      <Router>
        <Routes>
          <Route path="/dashboard/*" element={
            <Dashboard
              username={username}
              onLogout={() => {
                setIsLoggedIn(false);
                setUserId(null);
                setUsername('');
                setPassword('');
              }}
            />
          } />
          <Route path="*" element={<Navigate to="/dashboard" replace />} />
        </Routes>
      </Router>
    );
  }


  return (
    <div className="login-container">
      <div className="login-card">
        <div className="logo-container">
          <div className="logo-inner">
            <div className="logo-text">
              <h2>Risk Assessment Tool</h2>
            </div>
          </div>
          <h1>{isLogin ? 'Welcome Back!' : 'Create Account'}</h1>
          <p>{isLogin ? 'Please sign in to continue' : 'Sign up for a new account'}</p>
        </div>
        
        <form onSubmit={handleSubmit} className="form-container">
          <div className="input-group">
            <label className="input-label">Username</label>
            <input
              type="text"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              placeholder="Enter your username"
              required
              className="input-field"
            />
          </div>
          
          <div className="input-group">
            <label className="input-label">Password</label>
            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              placeholder="Enter your password"
              required
              className="input-field"
            />
          </div>

          <button
            type="submit"
            disabled={isLoading}
            className="submit-button"
          >
            {isLoading ? 'Processing...' : (isLogin ? 'Sign In' : 'Create Account')}
          </button>
        </form>

        {message && (
          <div className={`message ${message.includes('success') ? 'success' : 'error'}`}>
            {message}
          </div>
        )}

        <div className="toggle-auth">
          <button
            onClick={() => {
              setIsLogin(!isLogin);
              setMessage('');
            }}
            className="toggle-button"
          >
            {isLogin 
              ? "Don't have an account? Sign up" 
              : 'Already have an account? Sign in'}
          </button>
        </div>
      </div>
    </div>
  );
}

export default App;
