angular.module('codepuppy').controller('HeaderCtrl', [
'$scope', '$http', '$stateParams',
function($scope, $http, $stateParams)
{
    if ($stateParams.courseID) {
        $http({
            url: '/courses/' + $stateParams.courseID + '.json',
            method: 'GET',
            cache: true
        }).success(function(data, status, headers, config) {
            if (data.course_name) {
                $scope.title = data.course_code + ': ' + data.course_name;
            }
        });
    }
}]);
