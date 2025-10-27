export default function Footer() {
  return (
    <footer className="flex justify-between items-center px-6 py-4 bg-background text-foreground border-t border-border">
      <div className="text-sm">
        <p>© 2025 Mohamed Ourhouch | 42 Minishell Project</p>
      </div>
      <a
        href="https://github.com/ourhouchmohamed97"
        target="_blank"
        rel="noopener noreferrer"
        className="hover:text-muted-foreground transition-colors"
      >
        <svg
          className="w-5 h-5"
          fill="currentColor"
          viewBox="0 0 24 24"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path
            fillRule="evenodd"
            d="M12 0C5.37 0 0 5.37 0 12c0 5.3 3.438 9.8 8.205 11.385.6.111.82-.261.82-.58 0-.287-.011-1.244-.017-2.255-3.338.726-4.042-1.61-4.042-1.61-.546-1.387-1.333-1.756-1.333-1.756-1.09-.745.083-.73.083-.73 1.205.085 1.84 1.236 1.84 1.236 1.07 1.834 2.809 1.304 3.495.997.108-.776.42-1.304.762-1.604-2.665-.305-5.466-1.332-5.466-5.93 0-1.31.468-2.382 1.235-3.222-.124-.303-.536-1.524.117-3.176 0 0 1.008-.323 3.3 1.23a11.51 11.51 0 0 1 3.003-.403c1.02.005 2.047.137 3.003.403 2.29-1.553 3.297-1.23 3.297-1.23.655 1.652.243 2.873.119 3.176.77.84 1.233 1.912 1.233 3.222 0 4.61-2.804 5.624-5.476 5.921.43.372.823 1.103.823 2.222 0 1.606-.015 2.901-.015 3.293 0 .322.216.698.825.579C20.565 21.796 24 17.295 24 12 24 5.37 18.63 0 12 0z"
            clipRule="evenodd"
          />
        </svg>
      </a>
    </footer>
  );
}