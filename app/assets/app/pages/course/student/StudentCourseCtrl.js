angular.module('codepuppy').controller('StudentCourseCtrl', function($scope, $routeParams) {
    $scope.message = 'Look! I am a course page for ' + $routeParams.courseID;
});
