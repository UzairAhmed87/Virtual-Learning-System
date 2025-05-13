#ifndef SUPABASECONFIG_H
#define SUPABASECONFIG_H

#include <QString>

namespace SupabaseConfig {
// Supabase base URL
static QString getBaseUrl() {
    return "https://xubfmxtcefkzpfjytavd.supabase.co";
}

// Supabase API key (used for public access)
static QString getApiKey() {
    return "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDM2NzYyNDUsImV4cCI6MjA1OTI1MjI0NX0.JZxbHmLyV-Jigu0uQEql0xItr_vSQrUaKDCpNTJBFDQ";
}

// Supabase service role key (used for authenticated access)
static QString getServiceRoleKey() {
    return "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs";
}

// Storage URL for lectures
static QString getLectureStorageUrl() {
    return getBaseUrl() + "/storage/v1/object/lectures/";
}

static QString getAssignmentStorageUrl() {
    return getBaseUrl() + "/storage/v1/object/assignments/";
}

// Public storage URL for accessing uploaded lectures
static QString getPublicLectureUrl() {
    return getBaseUrl() + "/storage/v1/object/public/lectures/";
}

// REST API endpoint for file metadata
static QString getFilesApiUrl() {
    return getBaseUrl() + "/rest/v1/files";
}
}

#endif // SUPABASECONFIG_H
