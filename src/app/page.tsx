import Navbar from '@/components/Navbar';
import Terminal from '@/components/terminal';
import Footer from '@/components/footer';

export default function HomePage() {
  return (
    <div className="flex flex-col min-h-screen bg-background text-foreground font-mono">
      <Navbar />
      <main className="flex-1 flex items-center justify-center p-4">
        <Terminal />
      </main>
      <Footer />
    </div>
  )
}