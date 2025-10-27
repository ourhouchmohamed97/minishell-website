import { ModeToggle } from "./mode-toggle";
import Link from 'next/link'

export default function Navbar() {
  return (
    <nav className="flex justify-between items-center px-6 py-4 bg-background text-foreground border-b border-border">
      <div className="flex items-center space-x-4">
        <h1 className="text-xl font-bold">minishell</h1>
      </div>
      <div className="flex items-center space-x-6">
        <ul className="flex space-x-6">
          <li>
            <Link
              href="/terminal" 
              className="text-foreground hover:text-muted-foreground transition-colors duration-200 border-b-2 border-transparent hover:border-current"
            >
              Terminal
            </Link>
          </li>
          <li>
            <Link 
              href="/resources" 
              className="text-foreground hover:text-muted-foreground transition-colors duration-200 border-b-2 border-transparent hover:border-current"
            >
              Resources
            </Link>
          </li>
          <li>
            <Link
              href="https://github.com/ourhouchmohamed97/minishell" 
              target="_blank" 
              rel="noopener noreferrer" 
              className="text-foreground hover:text-muted-foreground transition-colors duration-200 border-b-2 border-transparent hover:border-current"
            >
              GitHub Repo
            </Link>
          </li>
        </ul>
      </div>
      <div className="flex items-center space-x-4">
        <ModeToggle />
      </div>
    </nav>
  );
}
