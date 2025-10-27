import Navbar from '@/components/Navbar';
import Footer from '@/components/footer';
import ResourcesPage from '@/components/resources';

export default function HomePage() {
  return (
    <div className="flex flex-col min-h-screen bg-background text-foreground font-mono">
      <Navbar />
      <main className="flex-1 flex items-center justify-center p-4">
        <ResourcesPage />
      </main>
      <Footer />
    </div>
  )
}