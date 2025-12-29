/**
 * login.js
 * Login page logic for Smart Home application
 * Handles user authentication (login/register)
 */

import { auth } from './firebase-config.js';
import { 
    signInWithEmailAndPassword, 
    createUserWithEmailAndPassword, 
    onAuthStateChanged 
} from "https://www.gstatic.com/firebasejs/10.7.1/firebase-auth.js";

// DOM Elements
const authForm = document.getElementById('auth-form');
const emailInput = document.getElementById('email');
const passwordInput = document.getElementById('password');
const confirmPasswordInput = document.getElementById('confirm-password');
const title = document.getElementById('form-title');
const submitBtn = document.getElementById('submit-btn');
const toggleBtn = document.getElementById('toggle-btn');
const errorMsg = document.getElementById('error-message');
const toggleMsg = document.getElementById('toggle-msg');

// State: true = login mode, false = register mode
let isLoginMode = true;

/**
 * Initialize authentication state listener
 * Redirects to dashboard if user is already logged in
 */
function initAuthStateListener() {
    // Check if Firebase auth is available
    if (!auth) {
        console.warn('[Login] Firebase auth not initialized. Please configure Firebase.');
        showError({ message: 'Firebase chưa được cấu hình. Vui lòng thiết lập Firebase Key.' });
        return;
    }

    onAuthStateChanged(auth, (user) => {
        if (user) {
            console.log('[Login] User already authenticated, redirecting to dashboard');
            window.location.href = "index.html";
        }
    });
}

// Initialize auth state listener
initAuthStateListener();

/**
 * Toggle between login and register modes
 */
function toggleAuthMode() {
    isLoginMode = !isLoginMode;
    hideError();

    if (isLoginMode) {
        title.textContent = "Đăng Nhập";
        submitBtn.textContent = "Đăng Nhập";
        confirmPasswordInput.style.display = "none";
        confirmPasswordInput.required = false;
        toggleMsg.textContent = "Chưa có tài khoản?";
        toggleBtn.textContent = "Đăng ký ngay";
    } else {
        title.textContent = "Đăng Ký Tài Khoản";
        submitBtn.textContent = "Đăng Ký";
        confirmPasswordInput.style.display = "block";
        confirmPasswordInput.required = true;
        toggleMsg.textContent = "Đã có tài khoản?";
        toggleBtn.textContent = "Đăng nhập ngay";
    }
}

/**
 * Handle form submission for login/register
 * @param {Event} e - Form submit event
 */
async function handleAuthSubmit(e) {
    e.preventDefault();
    
    // Check if Firebase auth is available
    if (!auth) {
        showError({ message: 'Firebase chưa được cấu hình. Vui lòng thiết lập Firebase Key.' });
        return;
    }
    
    const email = emailInput.value.trim();
    const password = passwordInput.value;
    
    // Basic validation
    if (!email || !password) {
        showError({ message: 'Vui lòng nhập đầy đủ email và mật khẩu.' });
        return;
    }
    
    // Disable submit button during request
    submitBtn.disabled = true;
    submitBtn.textContent = isLoginMode ? 'Đang đăng nhập...' : 'Đang đăng ký...';
    
    try {
        if (isLoginMode) {
            // Login
            await signInWithEmailAndPassword(auth, email, password);
            // Redirect handled by onAuthStateChanged
        } else {
            // Register - validate password confirmation
            if (password !== confirmPasswordInput.value) {
                showError({ message: "Mật khẩu xác nhận không khớp." });
                return;
            }
            
            // Password strength check
            if (password.length < 6) {
                showError({ message: "Mật khẩu phải có ít nhất 6 ký tự." });
                return;
            }
            
            await createUserWithEmailAndPassword(auth, email, password);
            alert("Đăng ký thành công!");
            // Redirect handled by onAuthStateChanged
        }
    } catch (error) {
        showError(error);
    } finally {
        // Re-enable submit button
        submitBtn.disabled = false;
        submitBtn.textContent = isLoginMode ? 'Đăng Nhập' : 'Đăng Ký';
    }
}

/**
 * Display error message to user
 * @param {Object} error - Error object with code and/or message
 */
function showError(error) {
    errorMsg.style.display = 'block';
    console.error('[Login] Error:', error.code || error.message);
    
    // Map Firebase error codes to user-friendly messages
    const errorMessages = {
        'auth/invalid-email': 'Email không hợp lệ.',
        'auth/user-disabled': 'Tài khoản đã bị vô hiệu hóa.',
        'auth/user-not-found': 'Không tìm thấy tài khoản.',
        'auth/wrong-password': 'Mật khẩu không đúng.',
        'auth/email-already-in-use': 'Email đã được sử dụng.',
        'auth/weak-password': 'Mật khẩu quá yếu. Vui lòng chọn mật khẩu mạnh hơn.',
        'auth/invalid-credential': 'Email hoặc mật khẩu không đúng.',
        'auth/too-many-requests': 'Quá nhiều lần đăng nhập thất bại. Vui lòng thử lại sau.',
        'auth/network-request-failed': 'Lỗi kết nối mạng. Vui lòng kiểm tra internet.'
    };
    
    errorMsg.textContent = errorMessages[error.code] || error.message || 'Đã có lỗi xảy ra.';
}

/**
 * Hide error message
 */
function hideError() {
    errorMsg.style.display = 'none';
    errorMsg.textContent = '';
}

// Event Listeners
if (toggleBtn) {
    toggleBtn.addEventListener('click', toggleAuthMode);
}

if (authForm) {
    authForm.addEventListener('submit', handleAuthSubmit);
}