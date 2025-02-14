import React, { useState } from 'react';
import Dashboard from './dashboard';


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
      <Dashboard 
        username={username} 
        onLogout={() => {
          setIsLoggedIn(false);
          setUserId(null);
          setUsername('');
          setPassword('');
        }} 
      />
    );
  }

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-600 to-purple-700 flex items-center justify-center p-6">
      <div className="bg-white bg-opacity-95 backdrop-blur-lg rounded-2xl shadow-2xl p-8 w-full max-w-md">
        <div className="text-center mb-8">
          <div className="inline-block p-2 bg-gradient-to-r from-blue-500 to-purple-500 rounded-2xl mb-4">
            <div className="bg-white rounded-xl p-4">
              <h2 className="text-transparent bg-clip-text bg-gradient-to-r from-blue-500 to-purple-500 text-2xl font-bold">
                Risk Assessment Tool
              </h2>
            </div>
          </div>
          <h1 className="text-3xl font-bold text-gray-800">
            {isLogin ? 'Welcome Back!' : 'Create Account'}
          </h1>
          <p className="text-gray-600 mt-2">
            {isLogin ? 'Please sign in to continue' : 'Sign up for a new account'}
          </p>
        </div>
        
        <form onSubmit={handleSubmit} className="space-y-6">
          <div className="space-y-4">
            <div>
              <label className="block text-sm font-semibold text-gray-700 mb-2">
                Username
              </label>
              <input
                type="text"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                placeholder="Enter your username"
                required
                className="w-full px-4 py-3 rounded-xl border-2 border-gray-200 focus:border-blue-500 focus:ring-2 focus:ring-blue-200 outline-none transition-all duration-200"
              />
            </div>
            
            <div>
              <label className="block text-sm font-semibold text-gray-700 mb-2">
                Password
              </label>
              <input
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                placeholder="Enter your password"
                required
                className="w-full px-4 py-3 rounded-xl border-2 border-gray-200 focus:border-blue-500 focus:ring-2 focus:ring-blue-200 outline-none transition-all duration-200"
              />
            </div>
          </div>

          <button 
            type="submit"
            disabled={isLoading}
            className="w-full bg-gradient-to-r from-blue-500 to-purple-500 text-white py-4 px-6 rounded-xl hover:from-blue-600 hover:to-purple-600 transition-all duration-200 font-semibold shadow-lg hover:shadow-xl disabled:opacity-50 disabled:cursor-not-allowed"
          >
            {isLoading ? 'Processing...' : (isLogin ? 'Sign In' : 'Create Account')}
          </button>
        </form>

        {message && (
          <div className={`mt-6 p-4 rounded-xl ${
            message.includes('success') 
              ? 'bg-green-50 text-green-800 border border-green-100' 
              : 'bg-red-50 text-red-800 border border-red-100'
          }`}>
            {message}
          </div>
        )}

        <div className="mt-8 text-center">
          <button
            onClick={() => {
              setIsLogin(!isLogin);
              setMessage('');
            }}
            className="text-transparent bg-clip-text bg-gradient-to-r from-blue-500 to-purple-500 font-semibold hover:from-blue-600 hover:to-purple-600 transition-colors duration-200"
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
