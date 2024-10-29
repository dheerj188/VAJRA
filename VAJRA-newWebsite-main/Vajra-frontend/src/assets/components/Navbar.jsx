/* eslint-disable react/prop-types */
"use client";
import { useState, useEffect, useRef } from "react";
import { motion } from "framer-motion";
import { FaBars } from "react-icons/fa"; // Burger icon
import { IoCloseSharp } from "react-icons/io5"; // Close icon

const transition = {
  type: "spring",
  mass: 0.5,
  damping: 11.5,
  stiffness: 100,
  restDelta: 0.001,
  restSpeed: 0.001,
};

const Navbar = () => {
  const [active, setActive] = useState(null);
  const [isOpen, setIsOpen] = useState(false); // State for mobile menu
  const [isDesktop, setIsDesktop] = useState(true); // Detect if the screen is desktop
  const navbarRef = useRef(); // Ref for detecting clicks outside of the navbar

  // Toggle dropdown menu for mobile
  const toggleMenu = () => {
    setIsOpen(!isOpen);
  };

  // Detect if the screen is larger than 1024px
  useEffect(() => {
    const handleResize = () => {
      setIsDesktop(window.innerWidth >= 1024);
    };

    // Initialize on mount
    handleResize();

    // Listen for resize events
    window.addEventListener("resize", handleResize);

    // Clean up event listener on unmount
    return () => window.removeEventListener("resize", handleResize);
  }, []);

  // Handle clicks outside of the navbar
  const handleClickOutside = (event) => {
    if (navbarRef.current && !navbarRef.current.contains(event.target)) {
      setActive(null); // Hide the dropdown if clicking outside
    }
  };

  // Add event listener for clicks outside
  useEffect(() => {
    document.addEventListener("mousedown", handleClickOutside);
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, []);

  return (
    <nav
      ref={navbarRef}
      style={{
        border: '1px solid var(--Card-Border, hsla(0, 0%, 100%, .06))',
        boxShadow: 'inset 0 1px 1px 0 hsla(0, 0%, 100%, .15)', // Corrected 'box-shadow' to 'boxShadow'
      }}
      className="fixed z-[999] top-10 w-[79%] mx-auto rounded-2xl backdrop-blur-sm bg-black/10 dark:bg-black/10 shadow-2xl border-2 border-gray-600/30 dark:border-gray-600/30 flex items-center justify-between px-12 py-5 text-gray-600 dark:text-gray-400"
    >
      {/* Always visible VAJRA */}
      <div
        onClick={() => setActive("VAJRA")}
        className="mr-4 font-bold text-xl cursor-pointer text-white"
      >
        VAJRA
      </div>

      {/* Links only visible on screens larger than 1024px */}
      <div className="hidden lg:flex space-x-20 items-center">
        <MenuItem setActive={setActive} active={active} item="Research" isDesktop={isDesktop}>
          Research
        </MenuItem>
        <MenuItem setActive={setActive} active={active} item="Company" isDesktop={isDesktop}>
          Company
        </MenuItem>
        <MenuItem setActive={setActive} active={active} item="Docs" isDesktop={isDesktop}>
          Docs
        </MenuItem>
      </div>

      {/* Mobile menu toggle */}
      <div className="lg:hidden">
        <button
          onClick={toggleMenu}
          className="text-white dark:text-gray-400"
          aria-label={isOpen ? "Close menu" : "Open menu"}
        >
          {isOpen ? <IoCloseSharp size={28} /> : <FaBars size={28} />}
        </button>
      </div>

      {/* Mobile dropdown menu (with interactive effects and high z-index) */}
      {isOpen && (
        <motion.div
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.3 }}
          className="absolute top-20 z-[1000] right-0 w-1/3 bg-black bg-opacity-50 border-2 border-white/[0.2] text-white p-4 rounded-xl lg:hidden"
        >
          <ul className="space-y-4 flex flex-col items-center">
            <MenuItem setActive={setActive} active={active} item="Research" isDesktop={isDesktop}>
              Research
            </MenuItem>
            <MenuItem setActive={setActive} active={active} item="Company" isDesktop={isDesktop}>
              Company
            </MenuItem>
            <MenuItem setActive={setActive} active={active} item="Docs" isDesktop={isDesktop}>
              Docs
            </MenuItem>
            <MenuItem setActive={setActive} active={active} item="Login" isDesktop={isDesktop}>
              Login
            </MenuItem>
          </ul>
        </motion.div>
      )}

      {/* Login - visible on large screens */}
      <MenuItem setActive={setActive} active={active} item="Login" className="ml-4 hidden lg:block" isDesktop={isDesktop}>
        Login
      </MenuItem>
    </nav>
  );
};

// MenuItem component with hover and click behavior
const MenuItem = ({ setActive, active, item, children, className, isDesktop }) => {
  const handleClick = () => {
    // Toggle active state on click
    setActive(active === item ? null : item);
  };

  return (
    <div
      onMouseEnter={() => isDesktop && setActive(item)} // Show dropdown on hover
      onMouseLeave={() => isDesktop && active !== item && setActive(null)} // Hide dropdown when not hovering, unless it's clicked
      onClick={handleClick} // Toggle active on click
      className={`relative cursor-pointer ${className}`}
    >
      <motion.p
        transition={{ duration: 0.3 }}
        className={`text-white dark:text-gray-400 hover:opacity-90 ${item === "VAJRA" ? "font-bold text-lg" : ""}`}
      >
        {item}
      </motion.p>

      {/* Dropdown only appears on hover or click */}
      {isDesktop && active === item && (
        <motion.div
          initial={{ opacity: 0, scale: 0.85, y: 10 }}
          animate={{ opacity: 1, scale: 1, y: 0 }}
          transition={transition}
          className="absolute top-[calc(100%_+_1.2rem)] left-[-1rem] transform -translate-x-1/2 pt-4"
        >
          <motion.div
            transition={transition}
            layoutId="active"
            className="bg-white dark:bg-black backdrop-blur-sm rounded-2xl overflow-hidden border border-black/[0.2] dark:border-white/[0.2] shadow-xl"
          >
            <motion.div layout className="w-max h-full p-4">
              {children}
            </motion.div>
          </motion.div>
        </motion.div>
      )}
    </div>
  );
};

export default Navbar;
