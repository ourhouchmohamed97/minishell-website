import Image from 'next/image';
import Link from 'next/link';

export default function ResourcesPage() {
  const resources = [
    {
      title: "Shunting Yard Algorithm",
      description: "Understand the Shunting Yard algorithm, essential for parsing mathematical expressions, applied in shell scripting and compilers.",
      buttonText: "Watch Tutorial",
      image: "/resources/shunting.jpeg",
      category: "Programming",
      url: "https://www.youtube.com/watch?v=unh6aK8WMwM&t=187s"
    },
    {
      title: "Signal Handling in C Tutorial",
      description: "Learn how to effectively handle signals in C programming, crucial for robust system-level applications like Minishell.",
      buttonText: "Watch Tutorial",
      image: "/resources/signals.jpeg",
      category: "System Programming",
      url: "https://www.youtube.com/watch?v=83M5-NPDeWs"
    },
    {
      title: "The Linux Programming Interface",
      description: "Comprehensive guide to Linux and UNIX system programming covering 500+ system calls, processes, signals, IPC, and network programming for Minishell development.",
      buttonText: "Read Documentation",
      image: "/resources/book.png",
      category: "Documentation",
      url: "https://broman.dev/download/The%20Linux%20Programming%20Interface.pdf"
    },    
    {
      title: "Bash Reference Manual",
      description: "Dive deep into the official GNU Bash Reference Manual. Essential for understanding shell commands and scripting.",
      buttonText: "Read Documentation",
      image: "/resources/gnu.jpg",
      category: "Documentation",
      url: "https://www.gnu.org/software/bash/manual/bash.html"
    },
    {
      title: "Process Management in Unix",
      description: "Explore process creation, execution, and termination in Unix-like systems, a core concept for Minishell development.",
      buttonText: "Watch Tutorial",
      image: "/resources/process.jpeg",
      category: "System Programming",
      url: "https://www.youtube.com/watch?v=OrM7nZcxXZU"
    },
    {
      title: "Unix Process Communication (fork, exec, pipe, dup2)",
      description: "Comprehensive explanation of inter-process communication and file descriptor duplication — the backbone of Minishell's process handling.",
      buttonText: "Read Tutorial",
      image: "/resources/ipc.jpeg",
      category: "System Programming",
      url: "https://tldp.org/LDP/lpg/node11.html"
    }
  ];

  return (
    <div className="min-h-screen bg-background text-foreground transition-colors duration-300">
      <main className="py-12 px-4 sm:px-6 lg:px-8">
        <div className="max-w-7xl mx-auto">
          {/* Header Section */}
          <div className="text-center mb-16">
            <h1 className="text-4xl font-bold text-foreground mb-4">
              Learning Resources
            </h1>
            <p className="text-lg text-gray-600 dark:text-gray-400 max-w-2xl mx-auto">
              Essential materials and tutorials to help you master Minishell development and system programming concepts.
            </p>
          </div>

          {/* Resources Grid - 3 columns, 2 rows */}
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
            {resources.map((resource, index) => (

              <div
                key={index}
                className="bg-transparent dark:bg-transparent rounded-xl shadow-lg border border-[#838CE5] overflow-hidden group hover:border-[#D6B9FC] transition-all duration-300 hover:shadow-xl hover:shadow-[#D6B9FC]/20 flex flex-col backdrop-blur-sm"
              >
                {/* Image Section */}
                <div className="relative h-60 bg-gradient-to-br from-[#838CE5] to-[#50207A] overflow-hidden">
                  <div className="relative h-60 bg-gray-200 dark:bg-gray-700 overflow-hidden">
                    <Image
                      src={resource.image}
                      alt={resource.title}
                      fill
                      className="object-cover group-hover:scale-105 transition-transform duration-300"
                      sizes="(max-width: 768px) 100vw, (max-width: 1200px) 50vw, 33vw"
                    />
                  </div>
                </div>
                {/* Content Section */}
                <div className="p-4 h-70 flex flex-col">
                  {/* Category Badge */}
                  <div className="w-fit px-3 py-1 bg-[#D6B9FC]/10 text-[#D6B9FC] text-xs font-medium rounded-full mb-4 border border-[#D6B9FC]/30">
                    {resource.category}
                  </div>

                  {/* Title */}
                  <h3 className="text-xl font-bold text-foreground mb-3 line-clamp-2 group-hover:text-[#838CE5] dark:group-hover:text-[#D6B9FC] transition-colors duration-300">
                    {resource.title}
                  </h3>

                  {/* Description */}
                  <p className="text-muted-foreground text-sm leading-relaxed mb-6 flex-grow line-clamp-3">
                    {resource.description}
                  </p>

                  {/* Button */}
                  <Link 
                    href={resource.url}
                    target="_blank"
                    rel="noopener noreferrer"
                    className="flex items-center justify-center w-full border-2 border-[#838CE5] text-[#838CE5] hover:bg-[#838CE5] dark:hover:bg-[#D6B9FC] hover:text-white font-semibold py-3 px-4 rounded-lg transition-all duration-300 transform hover:scale-105 active:scale-95">
                    {resource.buttonText}
                  </Link>
                </div>
              </div>
            ))}
          </div>
        </div>
      </main>
    </div>
  );
}
