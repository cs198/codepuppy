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
            url: '/courses/:courseID/assignments/:assignmentID/submit',
            templateUrl: '/assets/pages/fileUpload/' + role +
                '/upload.html',
            controller: 'FileUploadCtrl'
        });
    }

    // Leader-specific routes:
    if (role === 'leader') {
        $stateProvider.state('newAnnouncement', {
            url: '/courses/:courseID/newAnnouncement',
            templateUrl: '/assets/pages/newAnnouncement/' + role +
                '/announcement.html',
            controller: 'NewAnnouncementCtrl'
        });
    }

    $stateProvider.state('home', {
        url: '/',
        templateUrl: '/assets/pages/home/' + role + '/' + role + 'Home.html',
        controller: capRole + 'HomeCtrl'
    })
    .state('course', {
        url: '/courses/:courseID',
        templateUrl: '/assets/pages/course/' + role + '/' + role +
            'Course.html',
        controller: capRole + 'CourseCtrl'
    })
    .state('assignment', {
        url: '/courses/:courseID/assignments/:assignmentID',
        templateUrl: '/assets/pages/assignment/' + role + '/' + role +
            'Assignment.html',
        controller: capRole + 'AssignmentCtrl'
    })
    .state('submission', {
        url: '/courses/:courseID/assignments/:assignmentID/' +
             'submissions/:submissionID',
        templateUrl: '/assets/pages/submission/' + role + '/' + role +
            'Submission.html',
        controller: capRole + 'SubmissionCtrl'
    })
    .state('404', {
        url: '/404',
        templateUrl: '/assets/errorPages/404/404.html',
        controller: '404Ctrl'
    });

    $urlRouterProvider.otherwise('/404');
}).run(['session', function(session) {
    session.checkAuthenticated(function() {
        // no-op, success, continue to site
    }, function() {
        session.authenticate();
    });
}]);
