//= require_self
//= require_tree .

codepuppy = angular.module('codepuppy', ['ui.router', 'ngCookies',
                           'ui.bootstrap', 'angularFileUpload']);

codepuppy.config(function($stateProvider, $urlRouterProvider) {
    // TODO: use session information to determine role.  Should be one of
    // 'student', 'leader'
    var upperCaseFirst = function(str) {
        if (str.length === 0) {
            return str;
        }
        return str.charAt(0).toUpperCase() + str.slice(1);
    };

    var role = 'leader';
    var capRole = upperCaseFirst(role);

    // Student-specific routes:
    if (role === 'student') {
        $stateProvider.state('submit', {
            url: '^/courses/:courseID/assignments/:assignmentID/submit',
            views: {
                'content': {
                    templateUrl: '/assets/pages/fileUpload/' + role +
                        '/upload.html',
                    controller: 'FileUploadCtrl'
                },
                'sidebar': { template: '<sidebar />' },
                'header': { controller: 'HeaderCtrl' }
            }
        });
    }

    // Leader-specific routes:
    if (role === 'leader') {
        $stateProvider.state('newAnnouncement', {
            url: '^/courses/:courseID/newAnnouncement',
            views: {
                'content': {
                    templateUrl: '/assets/pages/newAnnouncement/' + role +
                        '/announcement.html',
                    controller: 'NewAnnouncementCtrl'
                },
                'sidebar': { template: '<sidebar />' },
                'header': { controller: 'HeaderCtrl' }
            }
        });
    }

    $stateProvider.state('home', {
        url: '^',
        views: {
            'content': {
                templateUrl: '/assets/pages/home/' + role + '/' + role +
                    'Home.html',
                controller: capRole + 'HomeCtrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    })
    .state('course', {
        url: '^/courses/:courseID',
        views: {
            'content': {
                templateUrl: '/assets/pages/course/' + role + '/' + role +
                    'Course.html',
                controller: capRole + 'CourseCtrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    })
    .state('assignment', {
        url: '^/courses/:courseID/assignments/:assignmentID',
        views: {
            'content': {
                templateUrl: '/assets/pages/assignment/' + role + '/' + role +
                    'Assignment.html',
                controller: capRole + 'AssignmentCtrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    })
    .state('submission', {
        url: '^/courses/:courseID/assignments/:assignmentID/' +
             'submissions/:submissionID',
        views: {
            'content': {
                templateUrl: '/assets/pages/submission/' + role + '/' + role +
                    'Submission.html',
                controller: capRole + 'SubmissionCtrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    })
    .state('submissionFile', {
        url: '/courses/:courseID/assignments/:assignmentID/' +
            'submissions/:submissionID/files/:submissionFileID',
        views: {
            'content': {
                templateUrl: '/assets/pages/submissionFile/' + role + '/' +
                    role + 'SubmissionFile.html',
                controller: capRole + 'SubmissionFileCtrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    })
    .state('404', {
        url: '^/404',
        views: {
            'content': {
                templateUrl: '/assets/errorPages/404/404.html',
                controller: '404Ctrl'
            },
            'sidebar': { template: '<sidebar />' },
            'header': { controller: 'HeaderCtrl' }
        }
    });

    $urlRouterProvider.otherwise('/404');
}).run(['session', function(session) {
    session.checkAuthenticated(function() {
        // no-op, success, continue to site
    }, function() {
        session.authenticate();
    });
}]);
