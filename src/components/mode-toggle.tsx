
"use client";

import * as React from "react";
import { useTheme } from "next-themes";
import { Sun, Moon } from "lucide-react";

import { Button } from "@/components/ui/button";

export function ModeToggle() {
  const { theme, setTheme, systemTheme } = useTheme();

  const [mounted, setMounted] = React.useState(false);

  React.useEffect(() => {
    setMounted(true);
  }, []);

  if (!mounted) {
    return null; // Avoid hydration mismatch
  }

  // Determine active theme considering system preference
  const currentTheme = theme === "system" ? systemTheme : theme;

  // Toggle between dark and light
  const toggleTheme = () => {
    setTheme(currentTheme === "dark" ? "light" : "dark");
  };

  return (
    <Button
      variant="outline"
      size="icon"
      aria-label="Toggle theme"
      title="Toggle theme"
      onClick={toggleTheme}
      className="relative"
    >
      <Sun
        className={`h-5 w-5 transition-transform duration-300 ease-in-out ${
          currentTheme === "dark" ? "scale-0 rotate-90" : "scale-100 rotate-0"
        } absolute`}
      />
      <Moon
        className={`h-5 w-5 transition-transform duration-300 ease-in-out ${
          currentTheme === "dark" ? "scale-100 rotate-0" : "scale-0 -rotate-90"
        }`}
      />
      <span className="sr-only">Toggle theme</span>
    </Button>
  );
}
